#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Your request must start with: %s 'EPSILON' 'X'\n", argv[0]);
        return 1;
    }
    char* term1;
    char* term2;
    double eps = strtod(argv[1], &term1);
    double x = strtod(argv[2], &term2);
    if (*term1 != '\0' || *term2 != '\0') {
        printf("Incorrect input\n");
        return 1;
    }

    ld res_a, res_b, res_c, res_d;
    switch (sum_a(eps, x, &res_a)) {
        case sc_correct:
            printf("Result of calculation a: %.9LF\n", res_a);
            break;
        case sc_invalid_argument:
            printf("There is invalid argument, fix it ant try again\n");
            break;
        case sc_overflow:
            printf("Result of calculation before overflow: %.9LF\n", res_a);
            break;
        case sc_diverge:
            printf("Function a(x) is diverging\n");
            break;
    }
    switch (sum_b(eps, x, &res_b)) {
        case sc_correct:
            printf("Result of calculation b: %.9LF\n", res_b);
            break;
        case sc_invalid_argument:
            printf("There is invalid argument, fix it ant try again\n");
            break;
        case sc_overflow:
            printf("Result of calculation before overflow: %.9LF\n", res_b);
            break;
        case sc_diverge:
            printf("Function b(x) is diverging\n");
            break;
    }
    switch (sum_c(eps, x, &res_c)) {
        case sc_correct:
            printf("Result of calculation c: %.9LF\n", res_c);
            break;
        case sc_invalid_argument:
            printf("There is invalid argument, fix it ant try again\n");
            break;
        case sc_overflow:
            printf("Result of calculation before overflow: %.9LF\n", res_c);
            break;
        case sc_diverge:
            printf("Function c(x) is diverging\n");
            break;
    }
    switch (sum_d(eps, x, &res_d)) {
        case sc_correct:
            printf("Result of calculation d: %.9LF\n", res_d);
            break;
        case sc_invalid_argument:
            printf("There is invalid argument, fix it ant try again\n");
            break;
        case sc_overflow:
            printf("Result of calculation before overflow: %.9LF\n", res_d);
            break;
        case sc_diverge:
            printf("Function d(x) is diverging\n");
            break;
    }
    printf("Epsilon = %.9f\n", eps);
    return 0;
}
