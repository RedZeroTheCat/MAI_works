#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

enum input_status_code {
    isc_invalid_input,
    isc_argument_error,
    isc_memory_error,
    isc_valid
};

bool is_lower_letter(char c) {
    return 'a' <= c && c <= 'z';
}

bool is_letter(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool is_num(char c) {
    return '0' <= c && c <= '9';
}

char to_upper(char c) {
    if (!is_lower_letter(c)) {
        return c;
    }
    return c - 'a' + 'A';
}

int stringCompare(const char* string1, const char* string2) {
    while (*string1 != '\0' && *string2 != '\0') {
        if (*string1 != *string2) {
            return 0;
        }
        string1++;
        string2++;
    }

    if (*string1 != '\0' || *string2 != '\0') {
        return 0;
    }
    return 1;
}

enum input_status_code analyzer(int argc, char* argv[], char* flag) {
    if (argc < 3) {
        return isc_invalid_input;
    }

    char* entered_flag = argv[1];
    if (!stringCompare(entered_flag, "-l")
        && !stringCompare(entered_flag, "-r")
        && !stringCompare(entered_flag, "-u")
        && !stringCompare(entered_flag, "-n")
        && !stringCompare(entered_flag, "-c"))
        {
            return isc_invalid_input;
    }
    *flag = entered_flag[1];

    if (*flag == 'c' && argc < 4) {
        return isc_invalid_input;
    }
    return isc_valid;
}

int error_message(enum input_status_code str_match) {
    if (str_match == isc_invalid_input) {
        printf("Error: INVALID INPUT\n");
    }
    else if (str_match == isc_memory_error) {
        printf("Error: MEMORY ERROR\n");
    }
    else if (str_match == isc_argument_error) {
        printf("Error: INVALID ARGUMENT\n");
    }
    return 0;
}


enum input_status_code stringLen(char* string, size_t* length) {
    if (string == NULL) {
        return isc_argument_error;
    }

    *length = 0;
    while (string[*length] != '\0') {
        (*length)++;
    }
    return isc_valid;
}

enum input_status_code reverse_string(char* string, char** reversed) {
    if (string == NULL) {
        return isc_argument_error;
    }
    size_t length;

    enum input_status_code str_match = stringLen(string, &length);
    if (str_match != isc_valid) {
        return str_match;
    }

    *reversed = (char*)malloc(sizeof(char) * (length + 1));
    if (*reversed == NULL) {
        return isc_memory_error;
    }

    for (int i = length - 1; i > -1; i--) {
        (*reversed)[length - 1 - i] = string[i];
    }
    (*reversed)[length] = '\0';
    return isc_valid;
}

enum input_status_code upper_odds(char* string, char** upped_string) {
    if (string == NULL) {
        return isc_argument_error;
    }
    size_t length;

    enum input_status_code str_match = stringLen(string, &length);
    if (str_match != isc_valid) {
        return str_match;
    }

    *upped_string = (char*)malloc(sizeof(char) * (length + 1));
    if (*upped_string == NULL) {
        return isc_memory_error;
    }

    for (int i = 1; i < length + 1; i++) {
        if (i % 2 == 0) {
            (*upped_string)[i - 1] = to_upper(string[i - 1]);
        }
        else {
            (*upped_string)[i - 1] = string[i - 1];
        }
    }
    (*upped_string)[length] = '\0';
    return isc_valid;
}

enum input_status_code nums_first(char* string, char** nummed_string) {
    if (string == NULL) {
        return isc_argument_error;
    }
    size_t length;

    enum input_status_code str_match = stringLen(string, &length);
    if (str_match != isc_valid) {
        return str_match;
    }

    *nummed_string = (char*)malloc(sizeof(char) * (length + 1));
    if (*nummed_string == NULL) {
        return isc_memory_error;
    }

    int position = 0;
    for (int i = 0; i < length; i++) {
        if (is_num(string[i])) {
            (*nummed_string)[position++] = string[i];
        }
    }

    for (int i = 0; i < length; i++) {
        if (is_letter(string[i])) {
            (*nummed_string)[position++] = string[i];
        }
    }

    for (int i = 0; i < length; i++) {
        if (!is_num(string[i]) && !is_letter(string[i])) {
            (*nummed_string)[position++] = string[i];
        }
    }
    return isc_valid;
}

void swapper(char** state1, char** state2) {
    char* tmp = *state1;
    *state1 = *state2;
    *state2 = tmp;
}

enum input_status_code concatenation(char* strings[], int count, char** concatenated) {
    if (strings == NULL) {
        return isc_argument_error;
    }
    size_t total_length = 0;

    for (int i = 0; i < count && count != 1; i++) {
        int j = rand() % 4;
        swapper(&strings[i], &strings[j]);
    }

    for (int i = 0; i < count; i++) {
        size_t length;
        enum input_status_code str_match = stringLen(strings[i], &length);
        if (str_match != isc_valid) {
            return str_match;
        }
        total_length += length;
    }

    *concatenated = (char*)malloc(sizeof(char) * (total_length + 1));
    if (*concatenated == NULL) {
        return isc_memory_error;
    }

    int position = 0;
    for (int i = 0; i < count; i++) {
        size_t word_length = 0;
        enum input_status_code str_match = stringLen(strings[i], &word_length);
        if (str_match != isc_valid) {
            free(*concatenated);
            return str_match;
        }

        for (size_t j = 0; j < word_length; j++) {
            (*concatenated)[position++] = strings[i][j];
        }
    }
    return isc_valid;
}
