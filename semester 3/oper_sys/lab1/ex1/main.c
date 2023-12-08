#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    status_code state = validate_input(argc, argv);
    if (state != correct) {
        return 0;
    }
    char* path = argv[1];
    FILE* output = NULL;

    state = try_to_open(&output, path, "wb");
    if (state != correct) {
        return 0;
    }

    state = fwrite_task_file(output);
    fclose(output);

    state = try_to_open(&output, path, "rb");
    if (state != correct) {
        return 0;
    }

    state = fread_task_file(output);
    fclose(output);

    state = try_to_open(&output, path, "rb");
    if (state != correct) {
        return 0;
    }

    printf("Moving pointer for 3 bytes from start\n");
    state = fseek_task_file(&output);
    if (state != correct) {
        return 0;
    }

    char buffer[4];
    state = fread_4bytes_task_file(&output, buffer);
    if (state != correct) {
        return 0;
    }

    printf("\nIn buffer will be: \n");
    for (int i = 0; i < 4; i += 1) {
        printf("%u ", buffer[i]);
    }
    printf("\n");
    fclose(output);
    return 0;
}
