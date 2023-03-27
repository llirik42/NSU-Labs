#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

#define ERROR (-1)

int make_dir(const char* path) {
    return mkdir(path, 0);
}

int print_directory_content(const char* path) {
    const unsigned int path_len = strlen(path);

    DIR* directory = opendir(path);

    if (directory == NULL) {
        return ERROR;
    }

    struct dirent* file = readdir(directory);
    struct stat file_info;
    while (file) {
        unsigned int file_name_len = strlen(file->d_name);

        char buffer[path_len + file_name_len + 2];
        buffer[path_len] = '/';
        buffer[path_len + file_name_len + 1] = '\0';
        for (unsigned int i = 0 ; i < path_len; i++) {
            buffer[i] = path[i];
        }
        for (unsigned int i = 0; i < file_name_len; i++) {
            buffer[path_len + i + 1] = file->d_name[i];
        }

        if (stat(buffer, &file_info) == -1) {
            perror(file->d_name);
        }
        if (S_ISREG(file_info.st_mode)) {
            printf("-        ");
        }
        if (S_ISDIR(file_info.st_mode)) {
            printf("d        ");
        }
        printf("%lu       %s\n", file_info.st_nlink, file->d_name);

        file = readdir(directory);
    }

    return -1;
}

int main() {
    print_directory_content("../dir");

    return 0;
}
