#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define PRINT_VARIABLE(VAR) printf("%-20s%-20p%d\n", #VAR, &(VAR), VAR);
#define CHILD_PROCESS_ID 0

#define CODE 5

int global_variable = 1;

int main() {
    printf("%-20s%d\n\n","Start pid",  getpid());

    int local_variable = 2;

    printf("Before fork\n");
    PRINT_VARIABLE(global_variable)
    PRINT_VARIABLE(local_variable)
    printf("\n");

    const int pid = fork();

    if (pid == CHILD_PROCESS_ID) {
        printf("Child pid=%d    parent pid=%d\n", getpid(), getppid());
        printf("Child before changing\n");
        PRINT_VARIABLE(global_variable)
        PRINT_VARIABLE(local_variable)
        printf("\n\n");

        global_variable = 5;
        local_variable = 13;

        printf("Child after changing\n");
        PRINT_VARIABLE(global_variable)
        PRINT_VARIABLE(local_variable)
        printf("\n\n");

        printf("Child: I terminate with code %d...\n\n", CODE);
        
        _exit(CODE);
    } else {
        sleep(1);

        printf("Parent after changing\n");
        PRINT_VARIABLE(global_variable)
        PRINT_VARIABLE(local_variable)
        printf("\n\n");

        int status;
        wait(&status);

        if (WIFSIGNALED(status)) {
            printf("Parent: child terminated by signal %d\n\n", WTERMSIG(status));
        }

        printf("Parent: child terminated with code %d\n\n", WEXITSTATUS(status));
    }
}
