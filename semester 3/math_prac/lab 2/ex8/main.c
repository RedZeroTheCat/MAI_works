#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main() {
    enum input_status_code state;
    char* a = "0000";
    char* b = "000";
    char* c = "000000";
    int base = 10;
    int count = 3;
    char* result = sum_nums(&state, base, count, a, b, c);
    if (state == isc_memory_error) {
        printf("Memory error\n");
    }
    else if (state == isc_wrong_base) {
        printf("Wrong base entered\n");
    }
    else if (state == isc_negative_num) {
        printf("Negative numbers are unexpected\n");
    }
    else if (state == isc_diff_with_base) {
        printf("Entered num differs woth base\n");
    }
    else {
        printf("Sum of entered numbers: %s\n", result);
    }

    printf("\n");
    base = 35;
    count = 3;
    a = "00000000000000000000000000000000000000000000Z";
    b = "0000000000a1";
    c = "101";
    result = sum_nums(&state, base, count, a, b, c);
    if (state == isc_memory_error) {
        printf("Memory error\n");
    }
    else if (state == isc_wrong_base) {
        printf("Wrong base entered\n");
    }
    else if (state == isc_negative_num) {
        printf("Negative numbers are unexpected\n");
    }
    else if (state == isc_diff_with_base) {
        printf("Entered num differs woth base\n");
    }
    else {
        printf("Sum of entered numbers: %s\n", result);
    }
    return 0;
}
