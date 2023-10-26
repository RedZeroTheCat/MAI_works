#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

bool сheck_final_representation(double num, int base) {
    long long iterations = 0;
    long long denominator = 1;
    while (num != (int)num) {
        if (num > 1e9 || iterations > 1e9) {
            return false;
        }
        num *= base;
        iterations++;
        denominator *= base;
    }
    for (int i = 2; i <= denominator && i <= base; i++) {
        while (base % i == 0) {
            while (denominator % i == 0) {
                denominator /= i;
            }
            base /= i;
        }
    }
    if (denominator == 1) {
        return true;
    }
    return false;
}

void representation_check(int base, int count, ...) {
    va_list entered_nums;
    va_start(entered_nums, count);
    for (int i = 0; i < count; i++) {
        double num = va_arg(entered_nums, double);
        printf("For %.6lf representation in base %d is  ", num, base);
        if (сheck_final_representation(num, base)) {
            printf("FINITE\n");
        }
        else {
            printf("INFINITE\n");
        }
    }
    va_end(entered_nums);
}
