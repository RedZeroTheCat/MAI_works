#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int convert_from_dec(int num, int base) {
    int result = 0;
    int k = 1;
    while (num > 0) {
        int carry = num % base;
        result += carry * k;
        num /= base;
        k *= 10;
    }
    return result;
}

void rewrite_two_in_one(FILE* input1, FILE* input2, FILE* output) {
    char sym_f1 = fgetc(input1);
    char sym_f2 = fgetc(input2);
    bool is_EOF1 = feof(input1);
    bool is_EOF2 = feof(input2);

    while (true) {
        while (isspace(sym_f1)) {
            sym_f1 = fgetc(input1);
        }
        while (isspace(sym_f2)) {
            sym_f2 = fgetc(input2);
        }

        if (!isspace(sym_f1) && sym_f1 != EOF) {
            if (!is_EOF2 && feof(input2)) {
                fprintf(output, " ");
                is_EOF2 = true;
            }
            fprintf(output, "%c", sym_f1);
            sym_f1 = fgetc(input1);
        }
        // if (sym_f1 != EOF) {
        //     fprintf(output, " ");
        // }

        if (!isspace(sym_f2) && sym_f2 != EOF) {
            if (!is_EOF1 && feof(input1)) {
                fprintf(output, " ");
                is_EOF1 = true;
            }
            fprintf(output, "%c", sym_f2);
            sym_f2 = fgetc(input2);
        }
        // if (sym_f2 != EOF) {
        //     fprintf(output, " ");
        // }

        if (sym_f1 == EOF && sym_f2 == EOF) {
            break;
        }
    }
}

void alternate_lexems(FILE* input, FILE* output) {
    char sym = fgetc(input);
    int lexeme_count = 1;
    while (sym != EOF) {
        while (isspace(sym)) {
            sym = fgetc(input);
        }
        if (lexeme_count % 10 == 0) {
            while (!isspace(sym) && sym != EOF) {
                if (isalpha(sym)) {
                    int digit = toascii(tolower(sym));
                    fprintf(output, "%d", convert_from_dec(digit, 4));
                }
                else {
                    fprintf(output, "%c", sym);
                }
                sym = fgetc(input);
            }
        }
        else if (lexeme_count % 5 == 0 && lexeme_count % 10 != 0) {
            while (!isspace(sym) && sym != EOF) {
                int digit = toascii(sym);
                fprintf(output, "%d", convert_from_dec(digit, 8));
                sym = fgetc(input);
            }
        }
        else if (lexeme_count % 2 == 0 && lexeme_count % 10 != 0) {
            while (!isspace(sym) && sym != EOF) {
                if (isalpha(sym)) {
                    fprintf(output, "%c", tolower(sym));
                }
                else {
                    fprintf(output, "%c", sym);
                }
                sym = fgetc(input);
            }
        }
        else {
            while (!isspace(sym) && sym != EOF) {
                fprintf(output, "%c", sym);
                sym = fgetc(input);
            }
        }
        fprintf(output, " ");
        lexeme_count++;
    }
}
