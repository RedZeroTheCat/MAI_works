#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void remove_digits(FILE* input, FILE* output) {
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (!isdigit(symbol)) {
            fprintf(output, "%c", symbol);
        }
        symbol = fgetc(input);
    }
}

void count_letters(FILE* input, FILE* output) {
    int count = 0;
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (isalpha(symbol) && isascii(symbol)) {
            count++;
        }
        symbol = fgetc(input);
        if (symbol == '\n' || symbol == EOF) {
            fprintf(output, "%d\n", count);
            count = 0;
        }
    }
}

void count_non_letters_nums(FILE* input, FILE* output) {
    int count = 0;
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (!isalnum(symbol) && !isspace(symbol) && isascii(symbol)) {
            count++;
        }
        symbol = fgetc(input);
        if (symbol == '\n' || symbol == EOF) {
            fprintf(output, "%d\n", count);
            count = 0;
        }
    }
}

void remove_non_digits_with_hex(FILE* input, FILE* output) {
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (symbol == '\n') {
            fprintf(output, "\n");
            symbol = fgetc(input);
        }
        if (isspace(symbol)) {
            fprintf(output, " ");
        }
        else if (isdigit(symbol)) {
            fprintf(output, "%c", symbol);
        }
        else if (isascii(symbol)) {
            fprintf(output, "%02X", symbol);
        }
        symbol = fgetc(input);
    }
}
