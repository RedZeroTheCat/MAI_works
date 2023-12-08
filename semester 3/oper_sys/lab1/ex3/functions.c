#include <stdio.h>

typedef enum {
    correct,
    invalid_input,
    file_error,
    copy_error
} status_code;

void print_error(status_code state) {
    switch (state) {
        case invalid_input:
            printf("Input is incorrect\n");
            break;
        case file_error:
            printf("Unable to open file(-s)\n");
            break;
        case copy_error:
            printf("Copy process error\n");
            break;
        case correct:
            break;
    }
}

status_code check_input(int argc, char* argv[]) {
    if (argc != 3) {
        return invalid_input;
    }
    return correct;
}

status_code try_to_open(FILE** file, char* path, char* param) {
    *file = fopen(path, param);
    if (*file == NULL) {
        return file_error;
    }
    return correct;
}

status_code copy_file(FILE* input, FILE* output) {
    char buffer[8];
    size_t read_bytes;
    size_t written_bytes;
    while (!feof(input)) {
        read_bytes = fread(buffer, sizeof(char), sizeof(buffer), input);
        written_bytes = fwrite(buffer, sizeof(char), read_bytes, output);
        if (read_bytes != written_bytes) {
            return copy_error;
        }
    }
    return correct;
}
