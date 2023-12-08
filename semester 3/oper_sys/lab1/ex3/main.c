#include <bits/types/FILE.h>
#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    status_code state = check_input(argc, argv);
    if (state != correct) {
        print_error(state);
        return 1;
    }

    FILE* input = NULL;
    state = try_to_open(&input, argv[1], "r");
    if (state != correct) {
        print_error(state);
        return 1;
    }

    FILE* output = NULL;
    state = try_to_open(&output, argv[2], "w");
    if (state != correct) {
        fclose(input);
        print_error(state);
        return 1;
    }

    state = copy_file(input, output);
    if (state != correct) {
        fclose(input);
        fclose(output);
        print_error(state);
        return 1;
    }

    printf("Copied successfully\n");
    fclose(input);
    fclose(output);
}
