#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>

#include "queue.h"

#define RED "\033[41m"
#define NOCOLOR "\033[0m"

void set_cpu(int n) {
	int err;
	cpu_set_t cpuset;
	pthread_t tid = pthread_self();

	CPU_ZERO(&cpuset);
	CPU_SET(n, &cpuset);

	err = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
	if (err) {
		printf("set_cpu: pthread_setaffinity failed for cpu %d\n", n);
		return;
	}

	printf("set_cpu: set cpu %d\n", n);
}

void *reader(void *arg) {
	int expected = 0;
	queue_t *q = (queue_t *)arg;
	printf("reader [%d %d %d]\n", getpid(), getppid(), gettid());

	set_cpu(1);

	while (1) {
        int val = -1;
		int ok = queue_get(q, &val);
		if (!ok)
			continue;

		if (expected != val)
			printf(RED"ERROR: get value is %d but expected - %d" NOCOLOR "\n", val, expected);

		expected = val + 1;
    }
}

void *writer(void *arg) {
	int i = 0;
	queue_t *q = (queue_t *)arg;
	printf("writer [%d %d %d]\n", getpid(), getppid(), gettid());

	set_cpu(2);

	while (1) {
		int ok = queue_add(q, i);
		if (!ok)
			continue;
		i++;
	}
}

int main() {
	pthread_t tid;
	queue_t *q;
	int err;

	printf("main [%d %d %d]\n", getpid(), getppid(), gettid());

	q = queue_init(1000000);

    err = pthread_create(&tid, NULL, writer, q);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }

    sched_yield();

    err = pthread_create(&tid, NULL, reader, q);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	pthread_exit(NULL);
}
