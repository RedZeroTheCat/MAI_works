#include <stdio.h>

#include "functions.c"

int main() {
    double* coefs_g = NULL;
    double eps = 1e-6;
    int grade = 5;
    double a = -2.0;
    switch (find_coefs(eps, a, &coefs_g, grade, -112.0, 7.0, -2.0, 5.0, 3.0, 1.0)) {
        case isc_correct:
            print_coefs(coefs_g, grade);
            free(coefs_g);
            break;
        case isc_invalid_argument:
            printf("Incorrect argument\n");
            break;
        case isc_memory_error:
            printf("Memory error\n");
            break;
    }

    grade = 3;
    a = 2;
    printf("\n");
    switch (find_coefs(eps, a, &coefs_g, grade, 2.0, 0.1, 3.0, 4.0)) {
        case isc_correct:
            print_coefs(coefs_g, grade);
            free(coefs_g);
            break;
        case isc_invalid_argument:
            printf("Incorrect argument\n");
            break;
        case isc_memory_error:
            printf("Memory error\n");
            break;
    }

    grade = 3;
    a = 0;
    printf("\n");
    switch (find_coefs(eps, a, &coefs_g, grade, 2.0, 0.0, 3.0, 4.0)) {
        case isc_correct:
            print_coefs(coefs_g, grade);
            free(coefs_g);
            break;
        case isc_invalid_argument:
            printf("Incorrect argument\n");
            break;
        case isc_memory_error:
            printf("Memory error\n");
            break;
    }
    return 0;
}
