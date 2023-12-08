#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    status_code state = check_input(argc, argv);
    if (state != correct) {
        print_error(state);
        return 1;
    }

    FILE* file;
    state = try_to_open(&file, argv[1], "rb");
    if (state != correct) {
        print_error(state);
        return 1;
    }

    bool flag = false;
    if (strcmp(argv[2], "mask") == 0) {
        char hex_num[4];
        int l = strlen(argv[3]);
        for (int i = 0; i < 4; i++) {
            if (i < l && is_hex(argv[3][i])) {
                char c = argv[3][i];
                if ('0' <= c && c <= '9') {
                    c -= '0';
                }
                else if ('A' <= c && c <= 'F' || 'a' <= c && c <= 'f') {
                    c = toupper(c);
                    c -= 'A' + 10;
                }
                hex_num[3 - i] = c;
            }
            else {
                flag = true;
                hex_num[3 - i] = 0;
            }
        }
        mask_hex(file, hex_num);
        if (flag) {
            printf("Some numbers in mask was invalid, they were ignored\n");
        }
    }
    else if (strcmp(argv[2], "xor8") == 0) {
        xor8(file);
    }
    else if (strcmp(argv[2], "xor32") == 0) {
        xor32(file);
    }
    fclose(file);
    return 0;
}
