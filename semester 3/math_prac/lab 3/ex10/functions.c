#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFF_SIZE 256

typedef enum {
    correct,
    invalid_input,
    file_error,
    overflow_error
} status_code;

void print_error(status_code state) {
    switch (state) {
        case overflow_error:
            printf("Overflow error\n");
            break;
        case file_error:
            printf("Unable to open file\n");
            break;
        case invalid_input:
            printf("Invalid input\n");
            break;
        case correct:
            break;
    }
}

char* get_expression(char buffer[BUFF_SIZE], FILE* input) {
    memset(buffer, '\0', BUFF_SIZE);
    return fgets(buffer, BUFF_SIZE + 1, input);
}

status_code fprint_tree(const char* expression, FILE* output) {
    int depth = 0;
    int count_sym = 1;
    for (int i = 0; expression[i] != '\0'; i++) {
        if (expression[i] == '(') {
            depth++;
            count_sym++;
        }
        else if (expression[i] == ',') {
            count_sym++;
        }
        else if (expression[i] == ')') {
            depth--;
        }
        else if (isalpha(expression[i])) {
            count_sym--;
            if (count_sym < 0) {
                return invalid_input;
            }
            for (int i = 0; i < depth; i++) {
                fprintf(output, "\t");
            }
            fprintf(output, "\\ %c\n", expression[i]);
        }
    }
    fprintf(output, "\n\n");
    return correct;
}
