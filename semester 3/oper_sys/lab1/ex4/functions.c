#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    correct,
    invalid_input,
    file_error
} status_code;

void print_error(status_code state) {
    switch (state) {
        case invalid_input:
            printf("Input is incorrect\n");
            break;
        case file_error:
            printf("Unable to open file\n");
            break;
        case correct:
            break;
    }
}

bool is_hex(char c) {
    return ('0' <= c && c <= '9' || 'A'<= c && c <= 'F');
}

status_code check_input(int argc, char* argv[]) {
    if (argc == 4 && strcmp(argv[2], "mask") == 0 && strlen(argv[3]) <= 4) {
        return correct;
    }
    if (argc != 3) {
        return invalid_input;
    }
    return correct;
}

status_code try_to_open(FILE** file, char* path, char* param) {
    *file = fopen(path, param);
    if (*file == NULL) {
        return file_error;
    }
    return correct;
}



void xor8(FILE* file) {
    char result = 0x00;
    char byte;
    while (fread(&byte, sizeof(char), 1, file) == 1) {
        result = result ^ byte;
    }
    printf("Summing bytes by module 2: %d\n", result);
}

void xor32(FILE* file) {
    char res[4] = {0x00, 0x00, 0x00, 0x00};
    char bytes[4];
    size_t read = 0;
    while ((read = fread(bytes, sizeof(char), 4, file) > 0)) {
        if (read != 4) {
            for (int i = (int)read; i < 4; i++) {
                bytes[i] = 0x00;
            }
        }
        for (int i = 0; i < 4; i++) {
            res[i] = res[i] ^ bytes[i];
        }
    }
    printf("Summing four-bytes values by module 2: %X %X %X %X\n", res[0], res[1], res[2], res[3]);
}

void mask_hex(FILE* file, char mask[]) {
    char bytes[4];
    int count = 0;
    size_t read = 0;
    while ((read = fread(bytes, sizeof(char), 4, file)) > 0) {
        if (read != 4) {
            for (int i = (int)read; i < 4; i++) {
                bytes[i] = 0;
            }
        }
        int match_count = 0;
        for (int i = 0; i < 4; i++) {
            if (mask[i] == (bytes[i] & mask[i])) {
                match_count++;
            }
        }
        if (match_count == 4) {
            count++;
        }
    }
    printf("Number of bytes, equal to mask: %d\n", count);
}
