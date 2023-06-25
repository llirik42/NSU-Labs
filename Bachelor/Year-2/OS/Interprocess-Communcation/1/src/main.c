#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SUCCESS_CODE 0
#define ERROR_CODE 1
#define REGION_SIZE_BYTES (1024 * 1024)
#define MAX_VALUE 42
#define ITERATIONS_COUNT 1000
#define SHARED_FILE_NAME "shared_file"

int parent(unsigned int* region_start) {
    for (size_t j = 0; j < ITERATIONS_COUNT; j++) {
        unsigned int value = 0;
        for (size_t i = 0; i < REGION_SIZE_BYTES / sizeof(int); i++, value = (value + 1) % MAX_VALUE) {
            region_start[i] = value;
        }
    }

    int status;
    if (wait(&status) == -1) {
        perror("wait() failed");
    }

    if (munmap(region_start, REGION_SIZE_BYTES) == -1) {
        perror("munmap() failed");
    }

    return SUCCESS_CODE;
}

int child(const unsigned int* region_start) {
    unsigned int prev_value = 0;
    for (size_t j = 0; j < ITERATIONS_COUNT; j++) {
        for (size_t i = 0; i < REGION_SIZE_BYTES / sizeof(int); i++) {
            const unsigned int cur_value = region_start[i];

            // If it's first iteration
            if (j == 0 && i == 0) {
                continue;
            }

            if (prev_value == MAX_VALUE - 1) {
                if (cur_value != 0) {
                    fprintf(stderr, "Error: prev=%u cur=%u\n", prev_value, cur_value);
                }
            }
            else {
                if (cur_value != prev_value + 1) {
                    fprintf(stderr, "Error: prev=%u cur=%u\n", prev_value, cur_value);
                }
            }

            prev_value = cur_value;
        }
    }

    return SUCCESS_CODE;
}

int main() {
    if (access(SHARED_FILE_NAME, F_OK) == 0) {
        if (unlink(SHARED_FILE_NAME) == -1) {
            perror("unlink() failed");
            return ERROR_CODE;
        }
    }

    int shared_fd = open("shared_file", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (shared_fd == -1) {
        perror("open() failed");
        return ERROR_CODE;
    }

    if (ftruncate(shared_fd, REGION_SIZE_BYTES) == -1) {
        perror("ftruncate() failed");
        if (close(shared_fd) == -1) {
            perror("close() failed");
        }
        return ERROR_CODE;
    }

    unsigned int* region_start = mmap(
            NULL,
            REGION_SIZE_BYTES,
            PROT_READ | PROT_WRITE,
            MAP_SHARED_VALIDATE,
            shared_fd,
            0
    );

    // We can safely close fd here because regions in the process bind to struct file*, not fd

    if (close(shared_fd) == -1) {
        perror("close() failed");
    }

    const int pid = fork();

    // Parent
    if (pid > 0) {
        return parent(region_start);
    }

    // Child
    if (pid == 0) {
        return child(region_start);
    }

    // Fork() failed
    perror("fork() failed");

    if (munmap(region_start, REGION_SIZE_BYTES) == -1) {
        perror("munmap() failed");
    }

    return ERROR_CODE;
}
