#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ftw.h>
#include "operations.h"
#include "utils.h"

int make_dir(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    if (mkdir(path, S_IRWXU) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}

char get_file_type(mode_t mode) {
    switch (mode) {
        case DT_LNK: return 'l';
        case DT_REG: return '-';
        case DT_DIR: return 'd';
        case DT_FIFO: return 'p';
        case DT_CHR: return 'c';
        case DT_BLK: return 'b';
        case DT_SOCK: return 's';
        default: return 'u';
    }
}
int print_directory_content(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    DIR* directory = opendir(path);

    if (directory == NULL) {
        perror(path);
        return ERROR;
    }

    while (1) {
        /*
         * readdir() (3) can return NULL in two cases
         * 1) We reached the end of the directory stream
         * 2) Some error occurred during reading the next file (but in this case errno sets appropriately)
         *
         * So we have to distinguish these cases. That's why we set errno to zero and then checks if it's non-zero
         */
        errno = 0;
        const struct dirent* file = readdir(directory);

        if (errno) {
            perror(path);
            break;
        }

        if (file == NULL) {
            break;
        }

        printf("%c                %s\n", get_file_type(file->d_type),  file->d_name);
    }

    if (closedir(directory) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}

int handle(const char* path) {
    return remove_file_raw(path);
}
int remove_directory(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    if (nftw(path, (__nftw_func_t) handle, 0, FTW_DEPTH | FTW_PHYS) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}

int create_file(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    const int fd = creat(path, S_IRWXU);

    if (fd == ERROR) {
        perror(path);
        return ERROR;
    }

    if (close(fd) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}

int print_file_content(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    FILE* file = fopen(path, "r");

    if (file == NULL) {
        perror(path);
        return ERROR;
    }

    char buffer[BUFFER_SIZE + 1];
    while (!feof(file)) {
        const unsigned long read_count = fread(buffer, sizeof(char), BUFFER_SIZE, file);

        if (ferror(file)) {
            printf("%s: Error during reading file", path);
            break;
        }

        buffer[read_count] = '\0';
        printf("%s", buffer);
    }

    if (fclose(file) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}

int remove_file_raw(const char* path) {
    if (remove(path) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}
int remove_file(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    return remove_file_raw(argv[1]);
}

int create_symlink(int argc, char** argv) {
    if (argc != 3) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* original = argv[1];
    const char* link = argv[2];

    if (symlink(original, link) == ERROR) {
        char buffer[BUFFER_SIZE] = {'\0'};
        sprintf(buffer, "Error during creating symlink \"%s\" to file \"%s\"", link, original);
        perror(buffer);
        return ERROR;
    }

    return SUCCESS;
}

int print_symlink_content(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    char buffer[BUFFER_SIZE + 1];

    const long read_count = readlink(path, buffer, BUFFER_SIZE);

    if (read_count == ERROR) {
        perror(path);
        return ERROR;
    }

    buffer[read_count] = '\0';

    return printf("%s\n", buffer);
}

int print_symlink_source_content(int argc, char** argv) {
    return print_file_content(argc, argv);
}

int remove_symlink(int argc, char** argv) {
    return remove_file(argc, argv);
}

int create_hard_link_raw(const char* old_path, const char* new_path) {
    if (link(old_path, new_path) == ERROR) {
        char buffer[BUFFER_SIZE] = {'\0'};
        sprintf(buffer, "Error during creating hard link \"%s\" to file \"%s\"", new_path, old_path);
        perror(buffer);
        return ERROR;
    }

    return SUCCESS;
}
int create_hard_link(int argc, char** argv) {
    if (argc != 3) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    return create_hard_link_raw(argv[1], argv[2]);
}

int remove_hard_link(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    if (unlink(path) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}

void write_permissions(char* buffer, mode_t mode) {
    buffer[0] = mode & S_IRUSR ? 'r' : '-';
    buffer[1] = mode & S_IWUSR ? 'w' : '-';
    buffer[2] = mode & S_IXUSR ? 'x' : '-';

    buffer[3] = mode & S_IRGRP ? 'r' : '-';
    buffer[4] = mode & S_IWGRP ? 'w' : '-';
    buffer[5] = mode & S_IXGRP ? 'x' : '-';

    buffer[6] = mode & S_IROTH ? 'r' : '-';
    buffer[7] = mode & S_IWOTH ? 'w' : '-';
    buffer[8] = mode & S_IXOTH ? 'x' : '-';
}
int print_permission_and_hard_links(int argc, char** argv) {
    if (argc != 2) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];

    struct stat info;
    if (lstat(path, &info) == ERROR) {
        perror(path);
        return ERROR;
    }

    char buffer[10];
    buffer[9] = '\0';
    write_permissions(buffer, info.st_mode);

    return printf("%s         %lu\n", buffer, info.st_nlink);
}

mode_t get_mode_from_string(const char* permissions) {
    mode_t result = 0;

    if (permissions[0] == 'r') {
        result |= S_IRUSR;
    }
    else if (permissions[0] != '-') {
        return ERROR;
    }
    if (permissions[1] == 'w') {
        result |= S_IWUSR;
    }
    else if (permissions[1] != '-') {
        return ERROR;
    }
    if (permissions[2] == 'x') {
        result |= S_IXUSR;
    }
    else if (permissions[2] != '-') {
        return ERROR;
    }

    if (permissions[3] == 'r') {
        result |= S_IRGRP;
    }
    else if (permissions[3] != '-') {
        return ERROR;
    }
    if (permissions[4] == 'w') {
        result |= S_IWGRP;
    }
    else if (permissions[4] != '-') {
        return ERROR;
    }
    if (permissions[5] == 'x') {
        result |= S_IXGRP;
    }
    else if (permissions[5] != '-') {
        return ERROR;
    }

    if (permissions[6] == 'r') {
        result |= S_IROTH;
    }
    else if (permissions[6] != '-') {
        return ERROR;
    }
    if (permissions[7] == 'w') {
        result |= S_IWOTH;
    }
    else if (permissions[7] != '-') {
        return ERROR;
    }
    if (permissions[8] == 'x') {
        result |= S_IXOTH;
    }
    else if (permissions[8] != '-') {
        return ERROR;
    }

    return result;
}
int change_mode(int argc, char** argv) {
    if (argc != 3) {
        print_incorrect_number_of_args();
        return ERROR;
    }

    const char* path = argv[1];
    const char* permissions = argv[2];

    if (strlen(permissions) != 9) {
        printf("Incorrect permissions format");
        return ERROR;
    }

    mode_t mode = get_mode_from_string(permissions);

    if (mode == ERROR) {
        printf("Incorrect permissions format");
        return ERROR;
    }

    if (chmod(path, mode) == ERROR) {
        perror(path);
        return ERROR;
    }

    return SUCCESS;
}
