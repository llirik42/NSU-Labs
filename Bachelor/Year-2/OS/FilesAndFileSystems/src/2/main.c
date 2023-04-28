#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <getopt.h>
#include "config.h"

#define WIDTH 32

void print_help() {
    printf("USAGE [-c] [-h]\n\n");

    printf("OPTIONS\n    -c, --create-hard-links         create hard links\n    -h, --help"
           "                      help\n\n");

    printf("LINKS\n");

    for (unsigned int i = 0; i < OPERATIONS_COUNT; i++) {
        const struct Operation* operation = &OPERATIONS[i];
        unsigned int len = strlen(operation->hard_link_name);

        printf("    %s", operation->hard_link_name);
        for (unsigned int j = 0; j < WIDTH - len; j++) {
            printf(" ");
        }
        printf("%s\n", operation->description);
    }
}

int create_hard_links(const char* source) {
    for (unsigned int i = 0; i < OPERATIONS_COUNT; i++) {
        if (create_hard_link_raw(source, OPERATIONS[i].hard_link_name) == ERROR) {
            return ERROR;
        }
    }

    return SUCCESS;
}

int main(int argc, char** argv) {
    const struct option long_options[] = {
            {"help", 0, 0, 'h'},
            {"create-hard-links", 0, 0, 'c'},
    };

    const char* name = basename(argv[0]);

    // Matching operation by name
    for (unsigned int i = 0; i < OPERATIONS_COUNT; i++) {
        const struct Operation* operation = &OPERATIONS[i];
        if (strcmp(name, operation->hard_link_name) == 0) {
            return operation->operation(argc, argv);
        }
    }

    if (argc == 1) {
        print_see_help();
        return SUCCESS;
    }

    // Reading arguments of command line
    int c;
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "hc", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                print_help();
                break;

            case 'c':
                create_hard_links(argv[0]);
                break;

            default:
                print_see_help();
                return ERROR;
        }
    }
}

