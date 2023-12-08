#include <iso646.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <wait.h>

typedef enum {
    correct,
    invalid_input,
    file_error,
    fork_error
} status_code;

void print_error(status_code state) {
    switch (state) {
        case invalid_input:
            printf("Invalid input\n");
            break;
        case file_error:
            printf("Unable to open file\n");
            break;
        case fork_error:
            printf("Failed to start new process\n");
            break;
        case correct:
            break;
    }
}

status_code get_input(char* buffer, int buffer_size) {
    for (int i = 0; i < buffer_size; ++i) {
        buffer[i] = '\0';
    }
    if (!fgets(buffer, sizeof(char) * buffer_size, stdin)) {
        return invalid_input;
    }
    if (!(buffer[buffer_size - 1] == '\0' || buffer[buffer_size - 2] == '\n')) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return correct;
}

int search_substr(int file_count, const char* sub_string, ...) {
    va_list args;
    va_start(args, sub_string);
    const char* cur_file_name;

    for (int i = 0; i < file_count; ++i) {
        cur_file_name = va_arg(args, const char*);
        FILE* file = fopen(cur_file_name, "r");
        if (file == NULL) {
            print_error(file_error);
            va_end(args);
            return 0; // not found
        }

        int sub_len = strlen(sub_string);
        char line[129];
        int line_len = sizeof(line);
        for (int i = 0; i < line_len; ++i) {
            line[i] = '\0';
        }

        while (fgets(line, line_len, file) != NULL) {
            for (int j = 0; j < line_len - sub_len ; ++j) {
                if (line[j] == sub_string[0]) {
                    int found = 1;
                    for (int k = 1; k < sub_len; ++k) {
                        if (line[j + k] != sub_string[k]) {
                            found = 0;
                            break;
                        }
                    }
                    if (found) {
                        fclose(file);
                        va_end(args);
                        return 1; // found
                    }
                }
            }
            for (int i = 0; i < line_len; ++i) {
                line[i] = '\0';
            }
        }
        fclose(file);
        va_end(args);
        return 0; // not found
    }
    va_end(args);
    return 0; // not found
}
