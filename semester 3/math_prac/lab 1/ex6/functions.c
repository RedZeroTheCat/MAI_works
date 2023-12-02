#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define ld long double

typedef enum {
    sc_invalid,
    sc_wrong_argument,
    sc_correct
} status_code;

status_code str_to_ld(char* str, ld* res) {
    char* terminator;
    *res = strtold(str, &terminator);
    if (*terminator != '\0') {
        return sc_wrong_argument;
    }
    return sc_correct;
}

status_code check_input(int argc, char* argv[], ld* eps) {
    if (argc != 2) {
        return sc_invalid;
    }
    status_code state = str_to_ld(argv[1], eps);
    if (state != sc_correct) {
        return state;
    }
    return sc_correct;
}

ld integral_a(ld eps, ld x) {
    if (x == 0.0L) {
        return logl(1.0L + x) / (x + eps);
    }
    return logl(1.0L + x) / x;
}

ld integral_b(ld eps, ld x) {
    if (x < eps) {
        x = 1.0L;
    }
    return expl((-x * x) * 0.5L);
}

ld integral_c(ld eps, ld x) {
    if (x == 1.0L) {
        return logl(1.0L / (1.0L - x + eps));
    }
    return logl(1.0L / (1.0L - x));
}

ld integral_d(ld eps, ld x) {
    if (x < eps) {
        return powl(x + eps, x + eps);
    }
    return powl(x, x);
}

ld mid_point(ld eps, ld a, ld b, ld(*function)(ld, ld)) {
    int num_intervals = 1;
    ld integral_prev = 0.0L;
    ld integral = eps + 1.0L;

    while (fabsl(integral - integral_prev) > eps) {
        integral_prev = integral;
        ld length_interval = (b - a) / num_intervals;
        integral = 0.0L;

        for (int i = 0; i < num_intervals; ++i) {
            ld x_mid_point = a + (i + 0.5L) * length_interval;
            integral += function(eps, x_mid_point);
        }
        integral *= length_interval;
        num_intervals *= 2;
    }
    return integral;
}
