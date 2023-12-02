#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Your request mus include: %s <base>\n", argv[0]);
        return 1;
    }
    char* terminator;
    int base = strtol(argv[1], &terminator, 10);
    if (*terminator !='\0') {
        printf("Incorrect base entered, fix and try again\n");
        return 1;
    }
    if (2 > base || base > 36) {
        printf("Invalid value of base, it must belong to interval [2, 36]\n");
        return 1;
    }
    char* input_value;
    int dec_max = 0;
    bool started = false;
    while (true) {
        bool is_minus;
        status_code input = scan_num_char(&input_value, &is_minus);
        if (input == sc_memory_error) {
            printf("Memory error\n");
            return 1;
        }
        else if (input == sc_stop) {
            break;
        }
        if (!is_valid_input(input_value, base)) {
            printf("Invalid argument\n");
            free(input_value);
            return 1;
        }
        int dec_temp;
        status_code check_valid = convert_to_decimal(input_value, base, &dec_temp);
        if (check_valid == sc_invalid) {
            printf("Invalid argument\n");
            free(input_value);
            return 1;
        }
        else {
            if (abs(dec_temp) > abs(dec_max) || !started) {
                started = true;
                dec_max = dec_temp;
                if (is_minus) {
                    dec_max *= -1;
                    is_minus = false;
                }
            }
        }
        free(input_value);
    }
    if (!started) {
        printf("You didn't enter any number\n");
        return 0;
    }
    char* max_value;
    status_code from_dec = convert_from_decimal(dec_max, base, &max_value);
    if (from_dec == sc_memory_error) {
        printf("Memory error\n");
        return 1;
    }
    printf("Max input num in base %d equals: %s\n", base, max_value);
    char* max_nine_base = NULL;
    char* max_eighteen_base = NULL;
    char* max_twenty_seven_base = NULL;
    char* max_thirty_six_base = NULL;

    from_dec = convert_from_decimal(abs(dec_max), 9, &max_nine_base);
    if (from_dec == sc_memory_error) {
        printf("Memory error\n");
        return 1;
    }
    from_dec = convert_from_decimal(abs(dec_max), 18, &max_eighteen_base);
    if (from_dec == sc_memory_error) {
        printf("Memory error\n");
        free(max_nine_base);
        return 1;
    }
    from_dec = convert_from_decimal(abs(dec_max), 27, &max_twenty_seven_base);
    if (from_dec == sc_memory_error) {
        printf("Memory error\n");
        free(max_nine_base);
        free(max_eighteen_base);
        return 1;
    }
    from_dec = convert_from_decimal(abs(dec_max), 36, &max_thirty_six_base);
    if (from_dec == sc_memory_error) {
        printf("Memory error\n");
        free(max_nine_base);
        free(max_eighteen_base);
        free(max_twenty_seven_base);
        return 1;
    }
    printf("Max input num in base 9 equals: %s\n", max_nine_base);
    printf("Max input num in base 18 equals: %s\n", max_eighteen_base);
    printf("Max input num in base 27 equals: %s\n", max_twenty_seven_base);
    printf("Max input num in base 36 equals: %s\n", max_thirty_six_base);

    free(max_value);
    free(max_nine_base);
    free(max_eighteen_base);
    free(max_twenty_seven_base);
    free(max_thirty_six_base);
    return 0;
}
