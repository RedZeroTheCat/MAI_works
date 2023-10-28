#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main() {
    enum input_status_code state;
    char* a = "A";
    char* b = "B";
    char* c = "0000C";
    int base = 10;
    int count = 3;
    char* result = sum_nums(&state, base, count, a, b, c);
    if (state != isc_valid) {
        printf("Memory error\n");
    }
    else {
        printf("Sum of entered numbers: %s\n", result);
    }

    printf("\n");
    base = 36;
    count = 3;
    a = "00000000000000000000000000000000000000000000Z";
    b = "00000000001";
    c = "101";
    result = sum_nums(&state, base, count, a, b, c);
    if (state != isc_valid) {
        printf("Memory error\n");
    }
    else {
        printf("Sum of entered numbers: %s\n", result);
    }
    return 0;
}
