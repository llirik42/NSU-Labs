#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define SUCCESS_CODE 0
#define ERROR_CODE 1
#define MAX_VALUE 42
#define ITERATIONS_COUNT 1000

int parent(int read_fd, int write_fd) {
    if (close(read_fd) == -1) {
        perror("close(read_fd) failed");
    }

    for (size_t j = 0; j < ITERATIONS_COUNT; j++) {
        for (unsigned int i = 0; i < MAX_VALUE; i++) {
            if (write(write_fd, &i, sizeof(i)) == -1) {
                perror("write() failed");
                return ERROR_CODE;
            }
        }
    }

    int status;
    if (wait(&status) == -1) {
        perror("wait() failed");
    }

    if (close(write_fd) == -1) {
        perror("close(write_fd) failed");
    }

    return SUCCESS_CODE;
}

int child(int read_fd, int write_fd) {
    if (close(write_fd) == -1) {
        perror("close(write_fd) failed");
    }

    unsigned int prev_value = 0;
    for (size_t j = 0; j < ITERATIONS_COUNT; j++) {
        for (unsigned int i = 0; i < MAX_VALUE; i++) {
            unsigned int cur_value;
            if (read(read_fd, &cur_value, sizeof(cur_value)) == -1) {
                perror("write() failed");
                return ERROR_CODE;
            }

            // If it's first iteration
            if (i == 0 && j == 0) {
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

    if (close(read_fd) == -1) {
        perror("close(read_fd) failed");
    }

    return SUCCESS_CODE;
}

int main() {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe() failed");
        return ERROR_CODE;
    }

    const int read_fd = pipefd[0];
    const int write_fd = pipefd[1];

    const int pid = fork();

    // Parent that will only write
    if (pid > 0) {
        return parent(read_fd, write_fd);
    }

    // Child that will only read
    if (pid == 0) {
        return child(read_fd, write_fd);
    }

    // Fork() failed
    perror("fork() failed");
    if (close(read_fd) == -1) {
        perror("close(read_fd) failed");
    }
    if (close(write_fd) == -1) {
        perror("close(write_fd) failed");
    }

    return ERROR_CODE;
}
