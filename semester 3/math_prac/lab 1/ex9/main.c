#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Your request must include: %s <a> <b>\n", argv[0]);
        printf("Success of 9.1 required for start 9.2\n");
        return 1;
    }
    char* terminator1;
    char* terminator2;
    long a = strtol(argv[1], &terminator1, 10);
    if (*terminator1 !='\0') {
        printf("Incorrect <a> entered, fix and try again\n");
        printf("Success of 9.1 required for start 9.2\n");
        return 1;
    }
    long b = strtol(argv[2], &terminator2, 10);
    if (*terminator2 !='\0') {
        printf("Incorrect <b> entered, fix and try again\n");
        printf("Success of 9.1 required for start 9.2\n");
        return 1;
    }
    if (a > b) {
        printf("Incorrect, A must be less than B\n");
        printf("Success of 9.1 required for start 9.2\n");
        return 1;
    }

    srand(time(NULL));
    int const size = 128;
    long array[size];
    printf("////////////////[ 9.1 ]////////////////\n\n");

    fill_array(array, size, a, b);
    printf("Original array: ");
    print_array(array, size);
    printf("\n");

    swap_min_max(array, size);
    printf("Original array: ");
    print_array(array, size);
    printf("\n");

    printf("////////////////[ 9.2 ]////////////////\n\n");
    int size_a = rand() % (10000 - 10 + 1) + 10;
    int size_b = rand() % (10000 - 10 + 1) + 10;

    long* array_a;
    long* array_b;
    long* array_c;

    switch (generate_array(&array_a, size_a, -1000, 1000)) {
        case sc_correct:
            break;
        case sc_memory_error:
            printf("Memory allocation error\n");
            return 1;
    }
    switch (generate_array(&array_b, size_b, -1000, 1000)) {
        case sc_correct:
            break;
        case sc_memory_error:
            printf("Memory allocation error\n");
            free(array_a);
            return 1;
    }

    printf("Array A of size %d: ", size_a);
    print_array(array_a, size_a);
    printf("\n\n");

    printf("Array B of size %d: ", size_b);
    print_array(array_b, size_b);
    printf("\n\n");

    switch (combine_arrays(&array_c, array_a, array_b, size_a, size_b)) {
        case sc_correct:
            break;
        case sc_memory_error:
            printf("Memory allocation error\n");
            free(array_a);
            free(array_b);
            return 1;
    }
    printf("Array C of size %d: ", size_a);
    print_array(array_c, size_a);
    free(array_a);
    free(array_b);
    free(array_c);
    printf("\n");
    return 0;
}
