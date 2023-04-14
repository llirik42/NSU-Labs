#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

#define MAX_MAPS_RECORD_LENGTH 1024
#define MAX_REGION_NAME 256
#define SUCCESS 0
#define ERROR_CODE (-1)
#define MAX_PATH_LENGTH 1024
#define HELP_CODE 1
#define HEX_BASE 16

void print_help() {
    printf("USAGE [-p PID] [-h]\n\n");

    printf("OPTIONS\n    -p, --pid         pid of process (default=self)\n    -h, --help"
           "        help\n\n");
}

void print_see_help() {
    fprintf(stderr, "See --help (-h).\n");
}

void print_columns_titles() {
    printf("PAGE                     PRESENT        SWAPPED        FILE        SOFT_DIRTY        PHYSICAL_PAGE"
           "                       REGION\n");
}

void print_page_data(long page_index, u_int64_t page_data, const char* region_name) {
    const bool soft_dirty = (page_data >> 54) & 1;
    const bool file_page = (page_data >> 61) & 1;
    const bool swapped = (page_data >> 62) & 1;
    const bool present = (page_data >> 63) & 1;
    const u_int64_t physical_page_index = page_data & (((u_int64_t)1 << 54) - 1);
    printf("%-16lx         %d              %d              %d           %d                 %#-16lx                  %s",
           page_index, present, swapped, file_page, soft_dirty, physical_page_index, region_name);
}

unsigned long long stringToLongLong(const char* string) {
    const unsigned int str_len = strlen(string);

    long long result = 0;

    char* tmp = malloc(2);
    tmp[1] = '\0';
    for (unsigned int i = 0; i < str_len; i++) {
        tmp[0] = string[i];
        result = result * HEX_BASE + strtoll(tmp, NULL, HEX_BASE);
    }

    free(tmp);

    return result;
}

void get_region_name_from_record(const char* record, char* region_name) {
    const unsigned int record_len = strlen(record);
    unsigned int index_of_last_space = 0;

    for (unsigned int i = 0; i < record_len && record[i] != '/'; i++) {
        if (record[i] == ' ') {
            index_of_last_space = i;
        }
    }

    const unsigned int region_name_length = record_len - index_of_last_space;
    for (unsigned int i = 0; i < region_name_length; i++) {
        region_name[i] = record[i + index_of_last_space];
    }
    region_name[region_name_length] = '\0';
}

void get_region_start_and_end(const char* record, char* start, char* end) {
    unsigned int i = 0;
    for (; record[i] != '-'; i++) {
        start[i] = record[i];
    }
    for (unsigned int j = 0; record[j + i + 1] != ' '; j++) {
        end[j] = record[i + j + 1];
    }
}

int print_pages_data_in_region(FILE* page_map_file, long first_page, long pages_count, const char* region_name) {
    if (fseek(page_map_file, first_page * 8, SEEK_SET) == -1) {
        return ERROR_CODE;
    }

    u_int64_t page_data;
    for (long j = 0; j < pages_count; j++) {
        fread(&page_data, sizeof(page_data), 1, page_map_file);
        print_page_data(first_page + j, page_data, region_name);
    }

    return SUCCESS;
}

int parse_options(int argc, char** argv, char* maps_path, char* page_map_path) {
    strcpy(maps_path, "/proc/self/maps");
    strcpy(page_map_path, "/proc/self/pagemap");

    const struct option long_options[] = {
            {"help", 0, NULL, 'h'},
            {"pid", 1,  NULL, 'p'},
    };

    int c;
    int option_index;
    while ((c = getopt_long(argc, argv, "p:h", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                print_help();
                return HELP_CODE;

            case 'p':
                sprintf(maps_path, "/proc/%s/maps", optarg);
                sprintf(page_map_path, "/proc/%s/pagemap", optarg);
                break;

            default:
                print_see_help();
                return ERROR_CODE;
        }
    }

    return SUCCESS;
}

int main(int argc, char** argv) {
    char maps_path[MAX_PATH_LENGTH] = {'\0'};
    char page_map_path[MAX_PATH_LENGTH] = {'\0'};

    const int code_of_parsing = parse_options(argc, argv, maps_path, page_map_path);
    if (code_of_parsing == HELP_CODE) {
        return SUCCESS;
    }
    if (code_of_parsing == ERROR_CODE) {
        return ERROR_CODE;
    }

    FILE* maps_file = fopen(maps_path, "r");
    if (maps_file == NULL) {
        perror(maps_path);
        return ERROR_CODE;
    }

    FILE* page_map_file = fopen(page_map_path, "r");
    if (page_map_file == NULL) {
        perror(page_map_path);

        if (fclose(maps_file) == -1) {
            perror(maps_path);
        }

        return ERROR_CODE;
    }

    print_columns_titles();

    const long page_size = sysconf(_SC_PAGESIZE);
    char record[MAX_MAPS_RECORD_LENGTH + 1];
    while (!feof(maps_file)) {
        fgets(record, MAX_MAPS_RECORD_LENGTH, maps_file);

        char region_start_str[HEX_BASE + 1] = {'\0'};
        char region_end_str[HEX_BASE + 1] = {'\0'};
        get_region_start_and_end(record, region_start_str, region_end_str);

        char region_name[MAX_REGION_NAME] = {'\0'};
        get_region_name_from_record(record, region_name);

        const unsigned long long region_start = stringToLongLong(region_start_str);
        const unsigned long long region_end = stringToLongLong(region_end_str);

        const long first_page = (long) (region_start / page_size);
        const long last_page = (long) (region_end / page_size);
        const long pages_count = last_page - first_page;

        if (print_pages_data_in_region(page_map_file, first_page, pages_count, region_name) == ERROR_CODE) {
            perror(page_map_path);
            break;
        }
    }

    if (ferror(maps_file)) {
        perror(maps_path);
    }
    if (ferror(page_map_file)) {
        perror(page_map_path);
    }

    bool closing_error = false;
    if (fclose(maps_file) == -1) {
        perror(maps_path);
        closing_error = true;
    }
    if (fclose(page_map_file) == -1) {
        perror(page_map_path);
        closing_error = true;
    }

    return closing_error ? ERROR_CODE : SUCCESS;
}

