#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define ERROR_CODE 1

void async_signal_write(const char* message) {
    write(1, message, strlen(message));
    fsync(1);
}

void handler1(int sig) {
    async_signal_write("Handler1!\n");
}

void* first_routine() {
     sigset_t set;
     if (sigfillset(&set) == -1) {
         perror("first_routine: sigfillset() failed");
         return NULL;
     }

     const int err = pthread_sigmask(SIG_SETMASK, &set, NULL);
     if (err) {
         fprintf(stderr, "first_routine: pthread_sigmask() failed: %s\n", strerror(err));
         return NULL;
     }
    return NULL;

//    signal(SIGINT, handler1);
//
//    while (1) {
//        printf("1\n");
//        fflush(stdout);
//        sleep(1);
//    }
}

void handler2(int sig) {
    async_signal_write("Handler2!\n");
}

void* second_routine() {
    signal(SIGINT, handler2);

    while (1) {
        printf("2\n");
        fflush(stdout);
        sleep(1);
    }
}

void* third_routine() {
    sigset_t set;
    if (sigemptyset(&set) == -1) {
        perror("third_routine: sigemptyset() failed");
        return NULL;
    }
    if (sigaddset(&set, SIGQUIT) == -1) {
        perror("third_routine: sigaddset() failed");
        return NULL;
    }

    int sig;
    const int err = sigwait(&set, &sig);
    if (err) {
        fprintf(stderr, "third_routine: sigwait() failed: %s\n", strerror(err));
        return NULL;
    }
    while (1) {
        printf("3\n");
        fflush(stdout);
        sleep(1);
    }
}

int main() {
    pthread_t first_tid;
    pthread_t second_tid;
    pthread_t third_tid;

    const int err1 = pthread_create(&first_tid, NULL, first_routine, NULL);
    if (err1) {
        fprintf(stderr, "main: pthread_create(1) failed: %s\n", strerror(err1));
        return ERROR_CODE;
    }

    const int err2 = pthread_create(&second_tid, NULL, second_routine, NULL);
    if (err2) {
        fprintf(stderr, "main: pthread_create(2) failed: %s\n", strerror(err2));
        return ERROR_CODE;
    }

    const int err3 = pthread_create(&third_tid, NULL, third_routine, NULL);
    if (err3) {
        fprintf(stderr, "main: pthread_create(3) failed: %s\n", strerror(err3));
        return ERROR_CODE;
    }

    sleep(1);
    const int kill_err1 = pthread_kill(first_tid, SIGINT);
    if (kill_err1) {
        fprintf(stderr, "main: pthread_kill(1) failed: %s\n", strerror(kill_err1));
    }

    const int kill_err2 = pthread_kill(second_tid, SIGINT);
    if (kill_err2) {
        fprintf(stderr, "main: pthread_kill(2) failed: %s\n", strerror(kill_err2));
    }

    const int kill_err3 = pthread_kill(third_tid, SIGQUIT);
    if (kill_err3) {
        fprintf(stderr, "main: pthread_kill(3) failed: %s\n", strerror(kill_err3));
    }

    while (1) {
        printf("0\n");
        fflush(stdout);
        sleep(1);
    }
}
