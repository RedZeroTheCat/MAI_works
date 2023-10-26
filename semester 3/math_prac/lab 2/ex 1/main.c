#include <stddef.h>
#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[])
{
    char flag;
    enum input_status_code str_match = analyzer(argc, argv, &flag);
    if (str_match != isc_valid) {
        return error_message(str_match);
    }
    if (flag == 'l') {
        size_t length;
        str_match = stringLen(argv[2], &length);
        if (str_match != isc_valid) {
            return error_message(str_match);
        }
        printf("Length of string is: %ld\n", length);
    }
    else if (flag == 'r') {
        char* reversed;
        str_match = reverse_string(argv[2], &reversed);
        if (str_match != isc_valid) {
            return error_message(str_match);
        }
        printf("Reversed string is: \n%s\n", reversed);
        free(reversed);
    }

    else if (flag == 'u') {
        char* upped_string;
        str_match = upper_odds(argv[2], &upped_string);
        if (str_match != isc_valid) {
            return error_message(str_match);
        }
        printf("String with upped odds is: \n%s\n", upped_string);
        free(upped_string);
    }

    else if (flag == 'n') {
        char* nummed_string;
        str_match = nums_first(argv[2], &nummed_string);
        if (str_match != isc_valid) {
            return error_message(str_match);
        }
        printf("String with first-nums, second-letters, third-other: \n%s\n", nummed_string);
        free(nummed_string);
    }

    else if (flag == 'c') {
        char* ptr;
        size_t number = strtoll(argv[2], &ptr, 10);
        if (*ptr != '\0') {
            return error_message(isc_invalid_input);
        }

        char** strings = (char**)malloc(sizeof(char*) * (argc - 3));
        if (strings == NULL) {
            return error_message(isc_memory_error);
        }

        for (int i = 0; i < argc - 3; i++) {
            strings[i] = argv[3 + i];
        }

        char* concatenated;
        srand(number);
        str_match = concatenation(strings, argc - 3, &concatenated);
        if (str_match != isc_valid) {
            free(concatenated);
            return error_message(str_match);
        }
        printf("Concatenation of entered strings with seed [%ld]: \n%s\n", number, concatenated);
        free(concatenated);
        free(strings);
    }

}
