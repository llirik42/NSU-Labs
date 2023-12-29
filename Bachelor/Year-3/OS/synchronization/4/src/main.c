#define _GNU_SOURCE

#include <stdatomic.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <getopt.h>
#include <sched.h>
#include "utils.h"
#include "storage.h"

#define CREATE_THREAD(TID, ROUTINE, NAME, STORAGE) { \
    const int err = pthread_create(&TID, NULL, ROUTINE, STORAGE); \
    if (err) {                                 \
        printf("main : pthread_create(%s) failed: %s\n", NAME, strerror(err));  \
        abort();                      \
    }                                       \
}

typedef bool (*node_comparator_t) (const struct Node*, const struct Node*);
typedef bool (*string_comparator_t) (const char*, const char*);

unsigned int less_checks_counter = 0;
unsigned int less_checker_iterations = 0;
unsigned int equal_checks_counter = 0;
unsigned int equal_checker_iterations = 0;
unsigned int greater_checks_counter = 0;
unsigned int greater_checker_iterations = 0;
unsigned int swaps_counter = 0;

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
}

bool need_to_swap() {
    return rand() % 2;
}

void print_statistics() {
    printf("%-30s%d\n"
           "%-30s%d\n"
           "%-30s%d\n"
           "%-30s%d\n"
           "%-30s%d\n"
           "%-30s%d\n"
           "%-30s%d\n\n",

           "Less checks", less_checks_counter,
           "Less checker iterations", less_checker_iterations,
           "Equal checks", equal_checks_counter,
           "Equal checker iterations", equal_checker_iterations,
           "Greater checks", greater_checks_counter,
           "Greater checker iterations", greater_checker_iterations,
           "Swaps", swaps_counter
    );
}


// Comparison of values of nodes
bool less_strings_comparator(const char* s1, const char* s2) {
    return strlen(s1) < strlen(s2);
}

bool equal_strings_comparator(const char* s1, const char* s2) {
    return strlen(s1) == strlen(s2);
}

bool greater_strings_comparator(const char* s1, const char* s2) {
    return strlen(s1) > strlen(s2);
}


// Comparison of nodes
bool compare_nodes(const struct Node* n1, const struct Node* n2, string_comparator_t cmp) {
    char b1[MAX_STRING_LENGTH + 1] = {'\0'};
    char b2[MAX_STRING_LENGTH + 1] = {'\0'};
    get_node_value(n1, b1);
    get_node_value(n2, b2);
    return cmp(b1, b2);
}

bool less_comparator(const struct Node* n1, const struct Node* n2) {
    return compare_nodes(n1, n2, less_strings_comparator);
}

bool equal_comparator(const struct Node* n1, const struct Node* n2) {
    return compare_nodes(n1, n2, equal_strings_comparator);
}

bool greater_comparator(const struct Node* n1, const struct Node* n2) {
    return compare_nodes(n1, n2, greater_strings_comparator);
}


// Abstract checker
void checker(void* arg, node_comparator_t cmp, unsigned int* checks_counter, unsigned int* iterations_counter) {
    struct Storage* storage = arg;

    while (1) {
        rlock_storage(storage);
        struct Node* current_node = get_head(storage);
        rlock_node(current_node);
        unlock_storage(storage);

        struct Node* next_node = get_next(current_node);

        while (next_node != NULL) {
            if (cmp(current_node, next_node)) {
                atomic_store(checks_counter, *checks_counter + 1);
            }

            rlock_node(next_node);
            unlock_node(current_node);
            current_node = next_node;
            next_node = get_next(current_node);
        }

        unlock_node(current_node);
        atomic_store(iterations_counter, *iterations_counter + 1);
    }
}

void* less_checker(void* arg) {
    set_cpu(0);
    checker(arg, less_comparator, &less_checks_counter, &less_checker_iterations);
    return NULL;
}

void* equal_checker(void* arg) {
    set_cpu(1);
    checker(arg, equal_comparator, &equal_checks_counter, &equal_checker_iterations);
    return NULL;
}

void* greater_checker(void* arg) {
    set_cpu(2);
    checker(arg, greater_comparator, &greater_checks_counter, &greater_checker_iterations);
    return NULL;
}

void* swapper(void* arg) {
    set_cpu(3);

    struct Storage* storage = arg;

    while (1) {
        struct Node *current_node = NULL;
        wlock_storage(storage);

        while (1) {
            struct Node *next_node = current_node == NULL ? get_head(storage) : get_next(current_node);
            if (next_node == NULL) {
                unlock_node(current_node);
                break;
            }
            wlock_node(next_node);

            // current node == NULL -> we are looking at the first element and there are no elements before it
            // -> we locked storage -> we have to unlock it
            if (current_node == NULL) {
                unlock_storage(storage);
            }

            struct Node *next_next_node = get_next(next_node);
            if (next_next_node == NULL) {
                unlock_node(next_node);
                unlock_node(current_node);
                break;
            }
            wlock_node(next_next_node);

            if (need_to_swap()) {
                if (current_node != NULL) {
                    set_next(current_node, next_next_node);
                }
                set_next(next_node, get_next(next_next_node));
                set_next(next_next_node, next_node);

                atomic_store(&swaps_counter, swaps_counter + 1);
            }

            unlock_node(next_next_node);
            unlock_node(next_node);
            unlock_node(current_node);

            current_node = next_node;
            wlock_node(current_node);
        }
    }
}

void print_help() {
    printf("USAGE [-h] -s SIZE -n NUMBER -t TIME\n\n");

    printf("OPTIONS\n"
           "    -h, --help                help\n"
           "    -s, --storage-size        size of storage\n"
           "    -n, --swappers-number     number of threads that swap elements of storage\n"
           "    -t, work-time             time (in seconds) when program works\n"
    );
}

int main(int argc, char** argv) {
    if (argc == 1) {
        print_help();
        return 0;
    }

    const struct option long_options[] = {
            {"help", 0, 0, 'h'},
            {"storage-size", 1, 0, 's'},
            {"swappers-number", 1, 0, 'n'},
            {"work-time(seconds)", 1, 0, 't'},
    };

    unsigned int storage_size;
    unsigned int swappers_number;
    unsigned int work_time;

    int opt;
    int option_index = 0;
    while((opt = getopt_long(argc, argv, "hs:n:t:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 's':
                storage_size = strtoul(optarg, NULL, 10);
                break;
            case 'n':
                swappers_number = strtoul(optarg, NULL, 10);
                break;
            case 't':
                work_time = strtoul(optarg, NULL, 10);
                break;
            case 'h':
                print_help();
                return 0;
        }
    }

    printf("s=%d n=%d t=%d\n", storage_size, swappers_number, work_time);

    pthread_t less_checker_tid, equal_checker_tid, greater_checker_tid;
    pthread_t* swappers_tids = calloc(swappers_number, sizeof(pthread_t));

    struct Storage* storage = create_storage(storage_size);

    CREATE_THREAD(less_checker_tid, less_checker, "less_checker", storage)
    CREATE_THREAD(equal_checker_tid, equal_checker, "equal_checker", storage)
    CREATE_THREAD(greater_checker_tid, greater_checker, "greater_checker", storage)

    for (unsigned int i = 0; i < swappers_number; i++) {
        CREATE_THREAD(swappers_tids[i], swapper, "swapper", storage)
    }

    sleep(work_time);
    print_statistics();
    free(swappers_tids);
}
