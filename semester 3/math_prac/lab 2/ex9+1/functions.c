#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

enum input_status_code {
    isc_correct,
    isc_invalid_argument,
    isc_memory_error
};

void print_coefs(double coefs[], int grade) {
    for (int i = 0; i < grade + 1; i++) {
        printf("g%d = %f\n", i, coefs[i]);
    }
}

void diff_coefs(int grade, double coefs[]) {
    for (int i = 0; i < grade; i++) {
        coefs[i] = coefs[i + 1] * (i + 1);
    }
}

enum input_status_code solve_polynom(double* result, double x, int grade, double coefs[]) {
    *result = 0;
    int current_grade = 1;
    for (int i = 0; i < grade + 1; i++) {
        *result += coefs[i] * current_grade;
        current_grade *= x;
    }
    return isc_correct;
}

enum input_status_code find_coefs(double eps, double a, double** coefs_g, int grade, ...) {
    if (grade < 0) {
        return isc_invalid_argument;
    }

    (*coefs_g) = (double*)malloc(sizeof(double) * (grade + 1));
    if (*coefs_g == NULL) {
        return isc_memory_error;
    }

    va_list entered_coefs;
    va_start(entered_coefs, grade);
    double coefs[grade + 1];

    for (int i = 0; i < grade + 1; i++) {
        coefs[i] = va_arg(entered_coefs, double);
    }
    va_end(entered_coefs);

    double factorial_n = 1;
    for (int i = 0; i < grade + 1; i++) {
        double g_coef;
        enum input_status_code polynom_state = solve_polynom(&g_coef, a, grade - i, coefs);

        (*coefs_g)[i] = g_coef;
        if (i > 1) {
            factorial_n *= i;
            (*coefs_g)[i] /= factorial_n;
        }
        diff_coefs(grade - i, coefs);
    }
    return isc_correct;
}
