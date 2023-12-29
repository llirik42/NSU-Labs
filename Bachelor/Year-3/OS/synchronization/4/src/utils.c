#include <stdlib.h>
#include "utils.h"

unsigned int generate_random_value(unsigned int min_inclusive, unsigned int max_inclusive) {
    return (rand() % (max_inclusive - min_inclusive + 1)) + min_inclusive;
}

void generate_random_string_len(char* buffer, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        buffer[i] = (char)generate_random_value('a', 'z');
    }
}

void generate_random_string(char* buffer) {
    unsigned int result_length = generate_random_value(0, MAX_STRING_LENGTH);
    buffer[result_length] = '\0';
    generate_random_string_len(buffer, result_length);
}
