#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your request must start with %s <flag> <arguments>\n", argv[0]);
        return 1;
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with symbol '-' or '/'\n");
        return 1;
    }

    char flag = argv[1][1];
    double eps;
    bool result;
    bool stop;
    switch (flag) {
        case 'q':
            if (argc != 6) {
                printf("ERROR: wrong count of arguments for flag 'q'\n");
                return 1;
            }
            eps = atof(argv[2]);
            double coefs[3];
            coefs[0] = atof(argv[3]);
            coefs[1] = atof(argv[4]);
            coefs[2] = atof(argv[5]);

            quick_sort(coefs, 0, 2, eps);
            double root1, root2;
            do {
                switch (solve_sqr_equation(coefs, eps, &root1, &root2)) {
                    case esc_invalid_argument:
                        printf("There is invalid argument, fix and try again.\n\n");
                        break;
                    case esc_two_roots:
                        printf("For equation  %.3f*x^2 + (%.3f)*x + (%.3f) \n", coefs[0], coefs[1], coefs[2]);
                        printf("\tSolutions: x1 = %f, x2 = %f\n\n", root1, root2);
                        break;
                    case esc_one_root:
                        printf("For equation  %.3f*x^2 + (%.3f)*x + (%.3f) \n", coefs[0], coefs[1], coefs[2]);
                        printf("\tSolution: x = %f\n\n", root1);
                        break;
                    case esc_no_real_roots:
                        printf("For equation  %.3f*x^2 + (%.3f)*x + (%.3f) \n", coefs[0], coefs[1], coefs[2]);
                        printf("There is no solutions in Real numbers\n\n");
                        break;
                }
            } while (next_permutation(coefs, 3));
            break;
        case 'm':
            if (argc != 4) {
                printf("ERROR: wrong count of arguments for flag 'm'\n");
                return 1;
            }
            int num1 = atoi(argv[2]);
            int num2 = atoi(argv[3]);
            switch (is_multiply(num1, num2, &result)) {
                case sc_correct:
                    if (result) {
                        printf("Yes, %d is multiple of %d\n", num1, num2);
                    }
                    else {
                        printf("No, %d is not multiple of %d\n", num1, num2);
                    }
                    break;
                case sc_invalid_argument:
                    printf("There is invalid argument, fix and try again.\n");
                    break;
            }
            break;
        case 't':
            if (argc != 6) {
                printf("ERROR: wrong count of arguments for flag 't'\n");
                return 1;
            }
            double eps = atof(argv[2]);
            double sides[3];
            sides[0] = atof(argv[3]);
            sides[1] = atof(argv[4]);
            sides[2] = atof(argv[5]);
            quick_sort(sides, 0, 2, eps);
            do {
                switch (is_triangle(sides, eps, &result)) {
                    case sc_correct:
                        if ((sides[0] + sides[1] - sides[2]) < eps || fabs(sides[0] + sides[1] - sides[2]) < eps) {
                            printf("Triangle with this sides can not exist\n");
                            stop = true;
                            break;
                        }
                        if (result) {
                            printf("Yes, if a = %f, b = %f, c = %f\n\n", sides[0], sides[1], sides[2]);
                            stop = true;
                        }
                        break;
                    case sc_invalid_argument:
                        printf("There is invalid argument, fix and try again.\n");
                        stop = true;
                        break;
                }
                if (result || stop) {
                    break;
                }
            } while (next_permutation(sides, 3));
            if (!result && !stop) {
                printf("No, if a = %f, b = %f, c = %f\n\n", sides[0], sides[1], sides[2]);
            }
            break;
        default:
            printf("Unknown flag. Possible flags are:\n");
            printf("-q  or /q\n-m  or  /m\n-t  or /t\n");
            break;
    }
    return 0;
}
