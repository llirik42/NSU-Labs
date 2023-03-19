#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define SLASH '/'
#define NULL_TERMINATOR '\0'
#define ERROR (-1)
#define SUCCESS 0

#define EXECUTE(CALL, ERROR_VALUE) if ((CALL) == (ERROR_VALUE)) {perror(NULL); return ERROR;}
#define EXECUTE_AND_RETURN(DEST, CALL, ERROR_VALUE) { \
    typeof(DEST) tmp = (CALL);                        \
    if (tmp == ERROR_VALUE) {                         \
        perror(NULL);                                 \
        return ERROR;                                 \
    }                                                 \
    DEST = tmp;                                       \
}

void copy(const char* from, char* to, unsigned int from_start_index, unsigned int to_start_index,
          unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        to[to_start_index + i] = from[from_start_index + i];
    }
}

void copy_inverted(const char* from, char* to, unsigned int from_end_index, unsigned int to_start_index,
                   unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        to[to_start_index + i] = from[from_end_index - i - 1];
    }
}

void write_full_original_path(const char* original_path, const char* name, char* full_original_path) {
    const unsigned int path_length = strlen(original_path);
    const unsigned int name_length = strlen(name);

    copy(original_path, full_original_path, 0, 0, path_length);
    full_original_path[path_length] = SLASH;
    copy(name, full_original_path, 0, path_length + 1, name_length);
    full_original_path[path_length + name_length + 1] = NULL_TERMINATOR;
}
void write_full_inverted_path(const char* inverted_path, const char* name, char* full_inverted_path) {
    const unsigned int path_length = strlen(inverted_path);
    const unsigned int name_length = strlen(name);

    copy(inverted_path, full_inverted_path, 0, 0, path_length);
    full_inverted_path[path_length] = SLASH;
    copy_inverted(name,
                  full_inverted_path,
                  name_length,
                  path_length + 1,
                  name_length);
    full_inverted_path[path_length + name_length + 1] = NULL_TERMINATOR;
}
int read_file_and_write_inverted(FILE* input_file, FILE* output_file, long size) {
    for (long i = 0; i < size; i++) {
        int c;
        EXECUTE_AND_RETURN(c, getc(input_file), EOF)

        EXECUTE(fputc(c, output_file), EOF)

        if (i != size - 1) {
            EXECUTE(fseek(input_file, SEEK_SET - 2, SEEK_CUR), ERROR)
        }
    }

    return SUCCESS;
}

int invert_file(const char* original_path, const char* inverted_path, const char* name) {
    const unsigned int path_length = strlen(original_path);
    const unsigned int name_length = strlen(name);

    char full_original_path[path_length + name_length + 2];
    char full_inverted_path[path_length + name_length + 2];

    write_full_original_path(original_path, name, full_original_path);
    write_full_inverted_path(inverted_path, name, full_inverted_path);

    FILE* input_file;
    EXECUTE_AND_RETURN(input_file, fopen(full_original_path, "r"), NULL)

    FILE* output_file;
    EXECUTE_AND_RETURN(output_file, fopen(full_inverted_path, "w"), NULL)

    EXECUTE(fseek(input_file, SEEK_SET, SEEK_END), ERROR)

    long size;
    EXECUTE_AND_RETURN(size, ftell(input_file), ERROR)

    EXECUTE(fseek(input_file, SEEK_SET - 1, SEEK_CUR), ERROR)

    read_file_and_write_inverted(input_file, output_file, size);

    EXECUTE(fclose(input_file), EOF)
    EXECUTE(fclose(output_file), EOF)

    return SUCCESS;
}

int make_dir(const char* path) {
    EXECUTE(mkdir(path, 0040000), ERROR)

    /*
     * (1 << 12) - 1 means all right for the others
     */
    static const int MODE = (1 << 12) - 1;
    EXECUTE(chmod(path, MODE), ERROR)

    return SUCCESS;
}

void write_inverted(const char* original, char* inverted, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        inverted[i] = original[length - i - 1];
    }
    inverted[length] = '\0';
}

bool is_dot_or_two_dots(const char* string) {
    return strcmp(string, ".") == 0 || strcmp(string, "..") == 0;
}

int handle_directory(const char* full_original_directory_path, const char* full_inverted_directory_path) {
    const unsigned int current_path_length = strlen(full_original_directory_path);

    DIR* original_directory;
    EXECUTE_AND_RETURN(original_directory, opendir(full_original_directory_path), NULL)

    struct dirent* current_file;
    EXECUTE_AND_RETURN(current_file, readdir(original_directory), (void*)ERROR)
    struct stat current_file_info;

    while (current_file) {
        if (is_dot_or_two_dots(current_file->d_name)) {
            current_file = readdir(original_directory);
            continue;
        }

        unsigned int current_file_name_length = strlen(current_file->d_name);

        char full_original_file_path[current_path_length + current_file_name_length + 2];
        char full_inverted_file_path[current_path_length + current_file_name_length + 2];

        write_full_original_path(full_original_directory_path,
                                 current_file->d_name,
                                 full_original_file_path);

        write_full_inverted_path(full_inverted_directory_path,
                                 current_file->d_name,
                                 full_inverted_file_path);

        EXECUTE(stat(full_original_file_path, &current_file_info), ERROR)

        if (S_ISDIR(current_file_info.st_mode)) {
            if (make_dir(full_inverted_file_path) == ERROR) {
                perror(NULL);
                break;
            }

            const int code_of_handle = handle_directory(full_original_file_path,
                                                        full_inverted_file_path);

            if (code_of_handle == ERROR) {
                perror(NULL);
                break;
            }
        }
        if (S_ISREG(current_file_info.st_mode)) {
            char inverted_name[current_file_name_length + 1];
            write_inverted(current_file->d_name, inverted_name, current_file_name_length);
            invert_file(full_original_directory_path,
                        full_inverted_directory_path,
                        current_file->d_name);
        }

        current_file = readdir(original_directory);
    }

    EXECUTE(closedir(original_directory), ERROR)

    return SUCCESS;
}

int get_index_of_last_slash(const char* string, unsigned int length) {
    /*
     * Returns index of last '/' or -1 if there is no '/' in the "string"
     */

    int result = (int) length - 1;
    while (result > -1 && string[result] != SLASH) {
        result--;
    }
    return result;
}
unsigned int get_effective_length(const char* string) {
    /*
     * If the "string" doesn't end with '/' returns strlen(string), else — strlen(string) - 1
     */

    unsigned int length = strlen(string);
    if (string[length - 1] == SLASH) {
        length--;
    }
    return length;
}
void write_inverted_path(const char* full_original_path, char* full_inverted_path, unsigned int length) {
    int index_of_last_slash = get_index_of_last_slash(full_original_path, length);

    if (index_of_last_slash != -1) {
        copy(full_original_path, full_inverted_path, 0, 0, index_of_last_slash);
        full_inverted_path[index_of_last_slash] = SLASH;
    }

    copy_inverted(full_original_path,
                  full_inverted_path,
                  length,
                  index_of_last_slash + 1,
                  length - index_of_last_slash - 1);

    full_inverted_path[length] = NULL_TERMINATOR;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments!\n");
        return 1;
    }
    if (argc > 2) {
        fprintf(stderr, "Too many arguments\n");
        return 2;
    }

    const char* full_original_path = argv[1];
    const unsigned int effective_length = get_effective_length(full_original_path);

    /*
     * If "full_original_path" was "a/b/c/dir1", then "full_inverted_path" will be "a/b/c/1rid"
     * If "full_original_path" was "a/b/c/dir1/", then "full_inverted_path" will be "a/b/c/1rid"
     * So basically, "full_original_path" splits into segments by '/' and the last segment inverts
     */
    char full_inverted_path[effective_length + 1];
    write_inverted_path(full_original_path, full_inverted_path, effective_length);

    EXECUTE(make_dir(full_inverted_path), ERROR)

    return handle_directory(full_original_path, full_inverted_path);
}
