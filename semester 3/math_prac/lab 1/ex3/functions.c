#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef enum equation_status_code {
    esc_one_root,
    esc_two_roots,
    esc_no_real_roots,
    esc_invalid_argument
} equation_status_code;

typedef enum status_code {
    sc_correct,
    sc_invalid_argument
} status_code;

void swap(double* array, int i, int j) {
    double tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void reverse(double* array, int begin, int end) {
    while (begin < end) {
        swap(array, begin, end);
        begin++;
        end--;
    }
}

void quick_sort(double* array, int low, int high, double eps) {
    if (low < high) {
        double pivot = array[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (array[j] - eps < pivot) {
                i++;
                swap(array, i, j);
            }
        }

        swap(array, i + 1, high);
        quick_sort(array, low, i, eps);
        quick_sort(array, i + 2, high, eps);
    }
}

bool next_permutation(double* array, int n) {
    int i = n - 2;
    while (i >= 0 && array[i] >= array[i + 1]) {
        i--;
    }
    if (i < 0) {
        return false;
    }

    int j = n - 1;
    while (array[j] <= array[i]) {
        j--;
    }
    swap(array, i, j);
    reverse(array, i + 1, n - 1);
    return true;
}

equation_status_code solve_sqr_equation(double* coefs, double eps, double* root1, double* root2) {
    if (fabs(coefs[0]) < eps) {
        return esc_invalid_argument;
    }
    double discriminant = coefs[1] * coefs[1] - 4 * coefs[0] * coefs[2];

    if (discriminant > 0) {
        *root1 = (-1 * coefs[1] + sqrt(discriminant)) / (2 * coefs[0]);
        *root2 = (-1 * coefs[1] - sqrt(discriminant)) / (2 * coefs[0]);
        return esc_two_roots;
    }
    else if (discriminant == 0) {
        *root1 = (-1 * coefs[1]) / (2 * coefs[0]);
        return esc_one_root;
    }
    else {
        return esc_no_real_roots;
    }
}

status_code is_multiply(int num1, int num2, bool* result) {
    if (num1 == 0 || num2 == 0) {
        return sc_invalid_argument;
    }
    if (num1 % num2 == 0) {
        *result = true;
    }
    else {
        *result = false;
    }
    return sc_correct;
}

status_code is_triangle(double* sides, double eps, bool* result) {
    if (fabs(sides[0]) < eps || fabs(sides[1]) < eps || fabs(sides[2]) < eps) {
        return sc_invalid_argument;
    }
    if (fabs(sides[0] * sides[0] - (sides[1] * sides[1] + sides[2] * sides[2])) < eps) {
        *result = true;
    }
    else {
        *result = false;
    }
    return sc_correct;
}
