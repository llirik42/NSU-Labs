#pragma once

#define ERROR (-1)
#define SUCCESS 0
#define BUFFER_SIZE 1024

struct Operation {
    int (*operation)(int, char**);
    const char* hard_link_name;
    const char* description;
};

void print_incorrect_number_of_args();

void print_see_help();
