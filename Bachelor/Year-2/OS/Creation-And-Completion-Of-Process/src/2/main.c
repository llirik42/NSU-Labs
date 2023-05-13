#include <unistd.h>
#include <stdio.h>

#define CHILD_PROCESS_ID 0

int main() {
    const int pid = fork();

    if (pid == CHILD_PROCESS_ID) {
        const int pid2 = fork();

        if (pid2 == CHILD_PROCESS_ID) {
            sleep(2);
            printf("%-30s%d\n", "Son", getpid());
            printf("%-30s%d\n", "Parent-1", getppid());
            fflush(stdout);
            sleep(10);
            printf("%-30s%d\n", "Parent-2", getppid());
            fflush(stdout);
            sleep(20);
        }
        else {
            sleep(1);
            printf("%-30s%d\n", "Father", getpid());
            fflush(stdout);
            sleep(10);
        }
    }
    else {
        printf("%-30s%d\n", "Grandfather", getpid());
        fflush(stdout);
        sleep(20);
    }
}
