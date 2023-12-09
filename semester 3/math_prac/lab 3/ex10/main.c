#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Your request must include: %s <input.file> <output.file>\n", argv[0]);
        return 1;
    }
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        print_error(file_error);
        fflush(stdout);
        return 1;
    }
    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        print_error(file_error);
        fclose(input);
        fflush(stdout);
        return 1;
    }

    char buffer[BUFF_SIZE];
    int count_trees = 1;
    while (get_expression(buffer, input) != NULL) {
        if (buffer[BUFF_SIZE - 1] != '\0') {
            print_error(overflow_error);
            fclose(input);
            fclose(output);
            fflush(stdout);
            return 1;
        }
        fprintf(output, "Expression %d tree:\n", count_trees);
        count_trees++;
        status_code state = fprint_tree(buffer, output);
        if (state != correct) {
            print_error(state);
            fclose(input);
            fclose(output);
            fflush(stdout);
            return 1;
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}
