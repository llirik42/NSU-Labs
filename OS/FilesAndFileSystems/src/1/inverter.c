#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

#define SLASH '/'
#define NULL_TERMINATOR '\0'
#define ERROR (-1)
#define SUCCESS 0

/*
 * Returns index of last '/' or -1 if there is no '/' in the "string"
 */
int get_index_of_last_slash(const char* string, unsigned int length) {
    int result = (int) length - 1;
    while (result > -1 && string[result] != SLASH) {
        result--;
    }
    return result;
}

/*
 * If the "string" doesn't end with '/' returns strlen(string), else — strlen(string) - 1
 */
unsigned int get_effective_length(const char* string) {
    unsigned int length = strlen(string);
    if (string[length - 1] == SLASH) {
        length--;
    }
    return length;
}

/*
 * Copies "count" bytes from one buffer to another and
 * to[to_start_index + i] = from[from_start_index + i];
 */
void copy(const char* from, char* to, unsigned int from_start_index, unsigned int to_start_index, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        memcpy(to + to_start_index, from + from_start_index, count);
    }
}

/*
 * Copies and inverts "count" bytes from one buffer to another and
 * to[to_start_index + i] = from[from_end_index - i - 1];
 */
void copy_inverted(const char* from, char* to, unsigned int from_end_index, unsigned int to_start_index, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        to[to_start_index + i] = from[from_end_index - i - 1];
    }
}

/*
 * Writes {original_path}/{name} to the "full_original_path" and sets the last char to '\0'
 */
void write_full_original_path(const char* original_path, const char* name, char* full_original_path) {
    const unsigned int path_length = strlen(original_path);
    const unsigned int name_length = strlen(name);

    copy(original_path, full_original_path, 0, 0, path_length);
    full_original_path[path_length] = SLASH;
    copy(name, full_original_path, 0, path_length + 1, name_length);
    full_original_path[path_length + name_length + 1] = NULL_TERMINATOR;
}

/*
 * Writes {original_path}/{<inverted name>} to the "full_inverted_path" and sets the last char to '\0'
 * where <inverted name> — is an inverted "name"
 */
void write_full_inverted_path(const char* inverted_path, const char* name, char* full_inverted_path) {
    const unsigned int path_length = strlen(inverted_path);
    const unsigned int name_length = strlen(name);

    copy(inverted_path, full_inverted_path, 0, 0, path_length);
    full_inverted_path[path_length] = SLASH;
    copy_inverted(name,
                  full_inverted_path,
                  name_length,
                  path_length + 1,
                  name_length);
    full_inverted_path[path_length + name_length + 1] = NULL_TERMINATOR;
}

/*
 * Inverts name of the starting directory and removes '/' at the end
 * Examples:
 * dir -> rid
 * a/b/c/d/dir -> a/b/c/d/rid
 * ../dir -> ../rid
 * dir/ -> rid
 * a/b/c/d/dir/ -> a/b/c/d/dir
 */
void write_start_inverted_path(const char* full_original_path, char* full_inverted_path, unsigned int length) {
    int index_of_last_slash = get_index_of_last_slash(full_original_path, length);

    if (index_of_last_slash != -1) {
        copy(full_original_path, full_inverted_path, 0, 0, index_of_last_slash);
        full_inverted_path[index_of_last_slash] = SLASH;
    }

    copy_inverted(full_original_path,
                  full_inverted_path,
                  length,
                  index_of_last_slash + 1,
                  length - index_of_last_slash - 1);

    full_inverted_path[length] = NULL_TERMINATOR;
}

/*
 * Reads "size" bytes from the "input_file" (from the end) and writes them to the "output_file" (from the start).
 * Preliminarily, it had to be "fseek to end - 1" on "input_file"
 */
int read_file_and_write_inverted(FILE* input_file, FILE* output_file, const char* input_file_name, const char* output_file_name, long size) {
    for (long i = 0; i < size; i++) {
        int c = getc(input_file);

        if (c == EOF) {
            perror(input_file_name);
            return ERROR;
        }

        if (fputc(c, output_file) == EOF) {
            perror(output_file_name);
            return ERROR;
        }

        if (i != size - 1 && fseek(input_file, SEEK_SET - 2, SEEK_CUR) == -1) {
            perror(input_file_name);
            return ERROR;
        }
    }

    return SUCCESS;
}

/*
 * Makes dir with the given path and mode
 */
int make_dir(const char* path, mode_t mode) {
    if (mkdir(path, mode) == -1) {
        perror(path);
        return ERROR;
    }
    return SUCCESS;
}

/*
 * Returns whether the "string" is "." or ".."
 */
bool is_dot_or_two_dots(const char* string) {
    return strcmp(string, ".") == 0 || strcmp(string, "..") == 0;
}

/*
 * Creates a file in the "inverted_path" with the inverted "name" and writes all bytes from {original_path}/{name}
 * there (inverted)
 */
int invert_file(const char* original_path, const char* inverted_path, const char* name) {
    const unsigned int path_length = strlen(original_path);
    const unsigned int name_length = strlen(name);

    char full_original_path[path_length + name_length + 2];
    char full_inverted_path[path_length + name_length + 2];

    write_full_original_path(original_path, name, full_original_path);
    write_full_inverted_path(inverted_path, name, full_inverted_path);

    FILE* input_file = fopen(full_original_path, "r");
    if (input_file == NULL) {
        perror(full_original_path);
        return ERROR;
    }

    FILE* output_file = fopen(full_inverted_path, "w");
    if (output_file == NULL) {
        perror(full_inverted_path);
        if (fclose(input_file) == EOF) {
            perror(full_original_path);
        }
        return ERROR;
    }

    if (fseek(input_file, SEEK_SET, SEEK_END) == -1) {
        if (fclose(input_file) == EOF) {
            perror(full_original_path);
        }
        if (fclose(output_file) == EOF) {
            perror(full_inverted_path);
        }
        return ERROR;
    }

    const long size = ftell(input_file);
    if (size == 0) {
        return SUCCESS;
    }

    if (size == -1) {
        if (fclose(input_file) == EOF) {
            perror(full_original_path);
        }

        if (fclose(output_file) == EOF) {
            perror(full_inverted_path);
        }

        return ERROR;
    }

    if (fseek(input_file, SEEK_SET - 1, SEEK_CUR) == -1) {
        if (fclose(input_file) == EOF) {
            perror(full_original_path);
        }

        if (fclose(output_file) == EOF) {
            perror(full_inverted_path);
        }

        return ERROR;
    }

    int code = read_file_and_write_inverted(input_file,
                                            output_file,
                                            full_original_path,
                                            full_inverted_path,
                                            size);
    if (code == ERROR) {
        if (fclose(input_file) == EOF) {
            perror(full_original_path);
        }
        if (fclose(output_file) == EOF) {
            perror(full_inverted_path);
        }
        return ERROR;
    }

    return SUCCESS;
}

/*
 * Reads the whole directory, inverts all regular files inside and calls handle_directory() for every subdirectory
 */
int handle_directory(const char* full_original_directory_path, const char* full_inverted_directory_path) {
    struct stat original_directory_info;
    if (stat(full_original_directory_path, &original_directory_info) == -1) {
        perror(full_original_directory_path);
        return ERROR;
    }
    if (make_dir(full_inverted_directory_path, original_directory_info.st_mode) == -1) {
        perror(full_inverted_directory_path);
        return ERROR;
    }

    const unsigned int current_path_length = strlen(full_original_directory_path);

    bool error = false;
    DIR* original_directory = opendir(full_original_directory_path);
    if (original_directory == NULL) {
        perror(full_original_directory_path);
        return ERROR;
    }

    struct dirent* current_file = readdir(original_directory);

    /*
     * On Linux there are always files "." and ".." in every directory.
     * That's why "current_file" can't be empty because of the empty directory and it only can be empty in case of an error.
     */
    if (current_file == NULL) {
        perror(full_original_directory_path);
        error = true;
    }

    struct stat current_file_info;
    while (current_file && !error) {
        if (is_dot_or_two_dots(current_file->d_name)) {
            current_file = readdir(original_directory);
            continue;
        }

        unsigned int current_file_name_length = strlen(current_file->d_name);

        char full_original_file_path[current_path_length + current_file_name_length + 2];
        char full_inverted_file_path[current_path_length + current_file_name_length + 2];

        write_full_original_path(full_original_directory_path,
                                 current_file->d_name,
                                 full_original_file_path);

        write_full_inverted_path(full_inverted_directory_path,
                                 current_file->d_name,
                                 full_inverted_file_path);

        if (stat(full_original_file_path, &current_file_info) == -1) {
            perror(full_original_file_path);
            error = true;
            break;
        }

        if (S_ISDIR(current_file_info.st_mode)) {
            const int code_of_handle = handle_directory(full_original_file_path,
                                                        full_inverted_file_path);

            if (code_of_handle == ERROR) {
                error = true;
                break;
            }
        }
        if (S_ISREG(current_file_info.st_mode)) {
            char inverted_name[current_file_name_length + 1];

            copy_inverted(current_file->d_name,
                          inverted_name,
                          current_file_name_length,
                          0,
                          current_file_name_length);

            inverted_name[current_file_name_length] = NULL_TERMINATOR;

            int code = invert_file(full_original_directory_path,
                                   full_inverted_directory_path,
                                   current_file->d_name);

            if (code == ERROR) {
                error = true;
                break;
            }
        }

        current_file = readdir(original_directory);
    }

    if (closedir(original_directory) == -1) {
        perror(full_original_directory_path);
        return ERROR;
    }

    return error ? ERROR : SUCCESS;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Not enough arguments!\n");
        return 1;
    }
    if (argc > 2) {
        fprintf(stderr, "Too many arguments\n");
        return 2;
    }

    const char* full_original_path = argv[1];
    const unsigned int effective_length = get_effective_length(full_original_path);

    /*
     * If "full_original_path" was "a/b/c/dir1", then "full_inverted_path" will be "a/b/c/1rid"
     * If "full_original_path" was "a/b/c/dir1/", then "full_inverted_path" will be "a/b/c/1rid"
     * So basically, "full_original_path" splits into segments by '/' and the last segment inverts
     */
    char full_inverted_path[effective_length + 1];
    write_start_inverted_path(full_original_path, full_inverted_path, effective_length);

    return handle_directory(full_original_path, full_inverted_path);
}
