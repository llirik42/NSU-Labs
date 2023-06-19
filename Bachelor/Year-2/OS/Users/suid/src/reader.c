#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define ERROR_CODE 1
#define SUCCESS_CODE 0
#define BUFFER_SIZE 1024

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: reader input_file_name\n");
        return ERROR_CODE;
    }

    printf("Real user-id = %d\n", getuid());
    printf("Effective user-id = %d\n", geteuid());

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen() failed");
        return ERROR_CODE;
    }

    bool error = false;

    char buffer[BUFFER_SIZE + 1];
    while (true) {
        unsigned long read_count = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        buffer[read_count] = '\0';
        printf("%s", buffer);

        if (feof(file)) {
            printf("\n");
            break;
        }

        if (ferror(file)) {
            printf("\n");
            perror("fread() failed");
            error = true;
            break;
        }
    }

    if (fclose(file)) {
        error = true;
        perror("fclose() failed");
    }

    return error ? ERROR_CODE : SUCCESS_CODE;
}
