#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define ll long long
#define ld long double

typedef enum {
    sc_invalid_argument,
    sc_diverge,
    sc_overflow,
    sc_correct
} status_code;

status_code factorial(int num, ld* res, ld eps) {
    if (num < 0) {
        res = NULL;
        return sc_invalid_argument;
    }
    if (num == 0 || num == 1) {
        *res = 1;
        return sc_correct;
    }
    ld prev_res = 1;
    *res = 1;
    for (int i = 2; i <= num; i++) {
        *res *= i;
        if (*res - prev_res < eps) {
            *res = prev_res;
            return sc_overflow;
        }
        prev_res = *res;
    }
    return sc_correct;
}

status_code double_factorial(int num, ld* res, double eps) {
    if (num < 0) {
        res = NULL;
        return sc_invalid_argument;
    }
    if (num == 0 || num == 1) {
        *res = 1;
        return sc_correct;
    }
    ll prev_res = 1;
    *res = 1;
    if (num % 2 == 0) {
        for (int i = 2; i <= num; i += 2) {
            *res *= i;
            if (*res - prev_res < eps) {
                *res = prev_res;
                return sc_overflow;
            }
            prev_res = *res;
        }
    }
    else {
        for (int i = 3; i <= num; i += 2) {
            *res *= i;
            if (*res < prev_res) {
                *res = prev_res;
                return sc_overflow;
            }
            prev_res = *res;
        }
    }
    return sc_correct;
}

status_code sum_a(double eps, double x, ld* res) {
    ld sum = 0;
    int n = 0;
    ld fact_res;
    ld pow_res;
    int iterations = 0;
    while (true) {
        if (iterations > 150) {
            return sc_diverge;
        }
        switch (factorial(n, &fact_res, eps)) {
            case sc_invalid_argument:
                return sc_invalid_argument;
            case sc_overflow:
                return sc_overflow;
            default:
                break;
        }
        pow_res = pow(x, n);
        if (n == 0) {
            *res = pow_res / fact_res;
        }
        else {
            sum = *res;
            *res += pow_res / fact_res;
            if (fabsl(sum - *res) < eps) {
                break;
            }
        }
        n++;
        iterations++;
    }
    return sc_correct;
}

status_code sum_b(double eps, double x, ld* res) {
    ld sum = 0;
    int n = 0;
    ld fact_res;
    double first_pow_res;
    ld second_pow_res;
    int iterations = 0;
    while (true) {
        if (iterations > 150) {
            return sc_diverge;
        }
        switch (factorial(2 * n, &fact_res, eps)) {
            case sc_invalid_argument:
                return sc_invalid_argument;
            case sc_overflow:
                return sc_overflow;
            default:
                break;
        }
        first_pow_res = pow(-1, n);
        second_pow_res = powl(x, 2 * n);
        if (n == 0) {
            *res = (first_pow_res * second_pow_res) / fact_res;
        }
        else {
            sum = *res;
            *res += (first_pow_res * second_pow_res) / fact_res;
            if (sum - *res < eps) {
                break;
            }
        }
        n++;
        iterations++;
    }
    return sc_correct;
}

status_code sum_c(double eps, double x, ld* res) {
    ld sum = 0;
    int n = 0;
    ld fact_res;
    ld first_pow_res;
    ld second_pow_res;
    ld third_pow_res;
    int iterations = 0;
    while (true) {
        if (iterations > 150) {
            return sc_diverge;
        }
        switch (factorial(3 * n, &fact_res, eps)) {
            case sc_invalid_argument:
                return sc_invalid_argument;
            case sc_overflow:
                return sc_overflow;
            default:
                break;
        }
        switch (factorial(n, &second_pow_res, eps)) {
            case sc_invalid_argument:
                return sc_invalid_argument;
            case sc_overflow:
                return sc_overflow;
            default:
                break;
        }
        first_pow_res = powl(3, 3 * n);
        second_pow_res = powl(second_pow_res, 3);
        third_pow_res = powl(x, 2 * n);
        if (n == 0) {
            *res = (first_pow_res * second_pow_res * third_pow_res) / fact_res;
        }
        else {
            sum = *res;
            *res += (first_pow_res * second_pow_res * third_pow_res) / fact_res;
            if (fabsl(sum - *res) < eps) {
                break;
            }
        }
        n++;
        iterations++;
    }
    return sc_correct;
}

status_code sum_d(double eps, double x, ld* res) {
    ld sum = 0;
    int n = 1;
    ld fact_res;
    ld first_pow_res;
    ld second_pow_res;
    ld third_pow_res;
    int iterations = 0;
    while (true) {
        if (iterations > 150) {
            return sc_diverge;
        }
        switch (double_factorial(2 * n, &fact_res, eps)) {
            case sc_invalid_argument:
                return sc_invalid_argument;
            case sc_overflow:
                return sc_overflow;
            default:
                break;
        }
        switch (double_factorial(2 * n - 1, &second_pow_res, eps)) {
            case sc_invalid_argument:
                return sc_invalid_argument;
            case sc_overflow:
                return sc_overflow;
            default:
                break;
        }
        first_pow_res = pow(-1, n);
        third_pow_res = powl(x, 2 * n);
        if (n == 0) {
            *res = (first_pow_res * second_pow_res * third_pow_res) / fact_res;
        }
        else {
            sum = *res;
            *res += (first_pow_res * second_pow_res * third_pow_res) / fact_res;
            if (fabsl(sum - *res) < eps) {
                break;
            }
        }
        n++;
        iterations++;
    }
    return sc_correct;
}
