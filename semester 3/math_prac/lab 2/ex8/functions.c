#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const char values[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

enum input_status_code {
    isc_memory_error,
    isc_valid
};

char* rm_leading_zeros(char* string) {
    int zeros_count = 0;
    long long length = strlen(string);
    while (string[zeros_count] == '0') {
        zeros_count++;
    }

    if (zeros_count == length) {
        string[0] = '0';
        string[1] = '\0';
        return string;
    }

    for (int i = 0; i < length - zeros_count; i++) {
        string[i] = string[i + zeros_count];
    }
    string[length - zeros_count] = '\0';
    return string;
}

int char_to_num(char symbol) {
    if (isdigit(symbol)) {
        return symbol - '0';
    }
    else {
        return tolower(symbol) - 'a' + 10;
    }
}

long long max(long long num1, long long num2) {
    if (num1 > num2) {
        return num1;
    }
    return num2;
}

void move_char_right(char* string, int length) {
    for (int i = length - 1; i >= 1; i--) {
        string[i] = string[i - 1];
    }
}

char* sum_two_num(enum input_status_code **state, int base, char* num1, char* num2) {
    long long len1 = strlen(num1);
    long long len2 = strlen(num2);
    long long max_length = max(len1, len2);

    char* result = (char*)malloc(sizeof(char) * (max_length + 1));
    if (result == NULL) {
        **state = isc_memory_error;
        return NULL;
    }

    long long excess = 0;
    for (int i = 1; max_length - i >= 0; i++) {
        int first = 0;
        int second = 0;
        int local_result;
        if (len1 - i >= 0) {
            first = char_to_num(num1[len1 - i]);
        }
        if (len2 - i >= 0) {
            second = char_to_num(num2[len2 - i]);
        }

        local_result = first + second + excess;

        result[max_length - i] = values[local_result % base];

        excess = local_result / base;
    }
    if (excess > 0) {
        char* new_result = (char*)realloc(result, sizeof(char) * (max_length + 2));
        if (new_result == NULL) {
            free(result);
            **state = isc_memory_error;
            return NULL;
        }
        result = new_result;
        move_char_right(result, max_length + 1);
        result[0] = values[excess];
        max_length++;
    }
    result[max_length] = '\0';
    **state = isc_valid;
    return result;
}

char* sum_nums(enum input_status_code *state, int base, int count, ...) {
    char* result = (char*)malloc(sizeof(char) * 2);
    if (result == NULL) {
        *state = isc_memory_error;
        return NULL;
    }
    result[0] = 0;
    result[1] = '\0';


    va_list ptr;
    va_start(ptr, count);
    for (int i = 0; i < count; ++i) {
        char* number = va_arg(ptr, char*);

        result = sum_two_num(&state, base, result, number);
        if (*state != isc_valid) {
            free(result);
            return NULL;
        }
    }
    va_end(ptr);
    rm_leading_zeros(result);
    return result;
}
