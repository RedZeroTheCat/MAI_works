#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        printf("Your request must consist of: %s 'flag' 'input_file'\n", argv[0]);
        return 1;
    }
    if (argc == 2) {
        printf("Your request must include more arguments\n");
        return 1;
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with '-' or '/'\n");
        return 1;
    }
    const char* input_name = argv[2];
    char* output_name = NULL;
    char flag;
    if (argv[1][1] == 'n') {
        output_name = argv[3];
        flag = argv[1][2];
        if (output_name == NULL) {
            printf("Your request must include path to the output file\n");
            return 1;
        }
    }
    else {
        flag = argv[1][1];
        const char* last_slash = strrchr(input_name, '/');
        char* prefix = "out_";
        output_name = (char*)malloc(strlen(input_name) + 5);
        if (output_name == NULL) {
            printf("Memory error\n");
            return 1;
        }
        if (last_slash != NULL) {
            strcpy(output_name, prefix);
            strcat(output_name, last_slash + 1);
        }
        else {
            strcpy(output_name, prefix);
            strcat(output_name, input_name);
        }
    }
    FILE* input_file = fopen(input_name, "r");
    if (input_file == NULL) {
        printf("Unable to open file\n");
        return 1;
    }
    FILE* output_file = fopen(output_name, "w");
    if (output_file == NULL) {
        printf("Unable to open file\n");
        fclose(input_file);
        return 1;
    }
    switch (flag) {
        case 'd':
            remove_digits(input_file, output_file);
            break;
        case 'i':
            count_letters(input_file, output_file);
            break;
        case 's':
            count_non_letters_nums(input_file, output_file);
            break;
        case 'a':
            remove_non_digits_with_hex(input_file, output_file);
            break;
        default:
            printf("Unknown flag. Possible flags:\n");
            printf("\t-d or /d\n\t-i or /i\n\t-s or /s\n\t-a or /a\n");
            break;
    }
    fclose(input_file);
    fclose(output_file);
    if (argc == 3) {
        free(output_name);
    }
    return 0;
}
