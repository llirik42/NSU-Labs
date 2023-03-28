#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ERROR (-1)

int make_dir(const char* path) {
    const int code = mkdir(path, S_IRWXU);

    if (code == -1) {
        perror(path);
    }

    return code;
}

int print_directory_content(const char* directory_path) {
    const unsigned int directory_path_len = strlen(directory_path);

    DIR* directory = opendir(directory_path);

    if (directory == NULL) {
        perror(directory_path);
        return ERROR;
    }

    struct dirent* file = readdir(directory);
    while (file) {
        const unsigned int file_name_len = strlen(file->d_name);
        char full_file_path[directory_path_len + file_name_len + 2];
        memcpy(full_file_path, directory_path, directory_path_len);
        memcpy(full_file_path + directory_path_len + 1, file->d_name, file_name_len);
        full_file_path[directory_path_len] = '/';
        full_file_path[directory_path_len + file_name_len + 1] = '\0';

        struct stat file_info;
        if (stat(full_file_path, &file_info) == -1) {
            perror(file->d_name);
        }

        mode_t mode = file_info.st_mode;

//        if (mode == DT_LNK) {
//            printf("l ");
//        }

        if (S_ISREG(mode)) {
            printf("-");
        }
        else if (S_ISDIR(mode)) {
            printf("d");
        }
        else if (S_ISFIFO(mode)) {
            printf("p");
        }
        else if (S_ISSOCK(mode)) {
            printf("s");
        }
        else if (S_ISLNK(mode)) {
            printf("l");
        }
        else if (S_ISCHR(mode)) {
            printf("c");
        }
        else if (S_ISBLK(mode)) {
            printf("b");
        }
        else {
            printf("u");
        }

        printf("        %s %ld\n",file->d_name, file_info.st_size);

        file = readdir(directory);
    }

    return -1;
}

int remove_file(const char* path) {
    const int code = unlink(path);

    if (code == -1) {
        perror(path);
    }

    return code;
}

int make_file(const char* path) {
    const int fd = creat(path, S_IRWXU);

    if (fd == -1) {
        perror(path);
        return ERROR;
    }

    const int code = close(fd);

    if (code == -1) {
        perror(path);
    }

    return code;
}








int main() {
    print_directory_content("../dir");

    //make_dir("../new_dir");
    return 0;
}
