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
#include <errno.h>

#define ERROR (-1)

int make_dir(const char* path) {
    const int code = mkdir(path, S_IRWXU);

    if (code == -1) {
        perror(path);
    }

    return code;
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
int print_directory_content(const char* directory_path) {
    DIR* directory = opendir(directory_path);

    if (directory == NULL) {
        perror(directory_path);
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
            perror(directory_path);
            break;
        }

        if (file == NULL) {
            break;
        }

        printf("%c                %s\n", get_file_type(file->d_type),  file->d_name);
    }

    const int closing_code = closedir(directory);
    if (closing_code == -1) {
        perror(directory_path);
    }

    return closing_code;
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
