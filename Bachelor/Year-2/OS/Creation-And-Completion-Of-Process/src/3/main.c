#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sched.h>
#include <sys/wait.h>
#include <stdbool.h>

#define SUCCESS_CODE 0
#define ERROR_CODE 1
#define STACK_FILE_PAGES_COUNT 1
#define STACK_FILE_NAME "stack"
#define RECURSION_DEPTH 3
#define BUFFER_SIZE 256

void recursive_function(unsigned char counter) {
    if (counter == 0) {
        return;
    }

    char c = counter + '0';
    char array[] = "Hello, world!\n";
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "From recursive function with %d", counter);
    printf("%-40s%p\n", buffer, __builtin_return_address(0));
    recursive_function(--counter);
}

int child() {
    printf("%-40s%d\n", "Child", getpid());
    recursive_function(RECURSION_DEPTH);
    printf("%-40s%p\n", "From main-child function", __builtin_return_address(0));
    return SUCCESS_CODE;
}

int main() {
    printf("%-40s%d\n", "Parent", getpid());

    const int stack_file_fd = open(STACK_FILE_NAME, O_RDWR | O_CREAT |O_TRUNC, S_IRWXU);
    if (stack_file_fd == -1) {
        perror("open() failed");
        exit(ERROR_CODE);
    }

    const long page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1) {
        perror("sysconf(_SC_PAGESIZE) failed");
        if (close(stack_file_fd) == -1) {
            perror("close() failed");
        }
        exit(ERROR_CODE);
    }

    const off_t stack_file_size = page_size * STACK_FILE_PAGES_COUNT;

    if (ftruncate(stack_file_fd, stack_file_size) == -1) {
        perror("ftruncate() failed");
        if (close(stack_file_fd) == -1) {
            perror("close() failed");
        }
        exit(ERROR_CODE);
    }

    void* ptr = mmap(NULL,
                     stack_file_size,
                     PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_SHARED_VALIDATE,
                     stack_file_fd,
                     0);

    // We can safely close fd here because regions in the process bind to file*, not fd
    if (close(stack_file_fd) == -1) {
        perror("Stack-file closing");
        exit(ERROR_CODE);
    }

    if (ptr == MAP_FAILED) {
        perror("mmap() failed");
        exit(ERROR_CODE);
    }

    bool error = false;
    bool cloned = true;

    // We have to specify SIGCHLD because otherwise parent process won't know if child process terminated or not
    if (clone(child, ptr + stack_file_size - 1, CLONE_FS | SIGCHLD, NULL) == -1) {
        cloned = false;
        perror("Clone() failed");
    }

    if (cloned) {
        int status;
        if (wait(&status) == -1) {
            perror("wait() failed");
            error = true;
        }
    }

    if (munmap(ptr, stack_file_size) == -1) {
        perror("munmap() failed");
        error = true;
    }

    return error ? ERROR_CODE : SUCCESS_CODE;
}
