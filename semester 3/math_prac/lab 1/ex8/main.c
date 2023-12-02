#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong amount of arguments\n");
        return 1;
    }
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Unable to open INPUT file\n");
        return 1;
    }
    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("Unable to open OUTPUT file\n");
        fclose(input);
        return 1;
    }
    int base;
    char* input_value;
    char* no_zeros_number;
    int dec_number;
    while (true) {
        bool is_minus = false;
        status_code check_input = scan_num_char(input, output, &input_value, &is_minus);
        if (check_input == sc_memory_error) {
            printf("Memory error\n");
            fclose(input);
            fclose(output);
            return 1;
        }
        else if (check_input == sc_stop) {
            break;
        }
        switch (check_min_base(input_value, &base)) {
            case sc_correct:
                fprintf(output, "min base = %d ", base);
                break;
            case sc_memory_error:
                printf("Memory error\n");
                fclose(input);
                fclose(output);
                return 1;
            default:
                printf("Invalid argument\n");
                free(input_value);
                fclose(input);
                fclose(output);
                return 1;
        }

        switch (convert_to_decimal(input_value, base, &dec_number)) {
            case sc_correct:
                if (is_minus) {
                    dec_number *= -1;
                }
                fprintf(output, "dec = %d ", dec_number);
                break;
            case sc_memory_error:
                printf("Memory error\n");
                fclose(input);
                fclose(output);
                return 1;
            default:
                printf("Invalid argument\n");
                free(input_value);
                fclose(input);
                fclose(output);
                return 1;
        }
        switch (convert_from_decimal(dec_number, base, &no_zeros_number)) {
            case sc_correct:
                fprintf(output, "no zeros = %s ", no_zeros_number);
                break;
            case sc_memory_error:
                printf("Memory error\n");
                fclose(input);
                fclose(output);
            default:
                printf("Invalid argument\n");
                free(input_value);
                fclose(input);
                fclose(output);
                return 1;
        }
        free(input_value);
        fprintf(output, "\n");
        free(no_zeros_number);
    }

    fclose(input);
    fclose(output);
    return 0;
}
