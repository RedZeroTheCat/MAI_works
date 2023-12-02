#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("There is not enough arguments in your request\n");
        return 1;
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with '-' or '/'\n");
        return 1;
    }

    char flag = argv[1][1];
    char* output_name;
    FILE* output_file;
    switch (flag) {
        case 'r':
            if (argc != 5) {
                printf("Request must include: %s -%c 'FILE_1' 'FILE_1' 'FILE_OUT'\n", argv[0], flag);
                break;
            }
            char* input_name1 = argv[2];
            char* input_name2 = argv[3];
            output_name = argv[4];
            FILE* input_file1 = fopen(input_name1, "r");
            if (input_file1 == NULL) {
                printf("Unable to open file\n");
                break;
            }
            FILE* input_file2 = fopen(input_name2, "r");
            if (input_file2 == NULL) {
                printf("Unable to open file\n");
                break;
            }
            output_file = fopen(output_name, "w");
            if (output_file == NULL) {
                printf("Incorrect path was entered\n");
                break;
            }

            rewrite_two_in_one(input_file1, input_file2, output_file);
            fclose(input_file1);
            fclose(input_file2);
            fclose(output_file);
            break;
        case 'a':
            if (argc != 4) {
                printf("Request must include: %s -%c 'FILE_1' 'FILE_1' 'FILE_OUT'\n", argv[0], flag);
                break;
            }
            char* input_name = argv[2];
            output_name = argv[3];
            FILE* input_file = fopen(input_name, "r");
            if (input_file == NULL) {
                printf("Unable to open file\n");
                break;
            }
            output_file = fopen(output_name, "w");
            if (output_file == NULL) {
                printf("Incorrect path was entered\n");
                break;
            }

            alternate_lexems(input_file, output_file);
            fclose(input_file);
            fclose(output_file);
            break;
        default:
            printf("You've entered unknown flag, possible flags:\n");
            printf("\t-r OR /r\n\t-a OR /a\n");
            break;
    }
    return 0;
}
