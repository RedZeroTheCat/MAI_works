#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define ONE 1.0L
#define LOWEST_EPS 1e-9
#define HIGHEST_EPS 1e-1
#define SIZE 50000

enum input_status_code {
    isc_correct_input,
    isc_wrong_input
};

long double string_to_ld(char* str) {
    char* terminator;
    long double num = strtold(str, &terminator);
    if (*terminator != '\0') {
        return 0;
    }
    return num;
}

bool is_correct_num(char* argv) {
    long double num = string_to_ld(argv);
    if (num < LOWEST_EPS || num > HIGHEST_EPS || num == 0) {
        return false;
    }
    return true;
}

enum input_status_code analyzer(int argc, char** argv) {
    if (argc == 2 && is_correct_num(argv[1])) {
        return isc_correct_input;
    }
    return isc_wrong_input;
};

long double bin_pow(long double x, long long pow) {
    if (pow == 0) {
        return 1;
    }
    if (pow % 2 == 0) {
        long double result = bin_pow(x, pow / 2);
        return result * result;
    }
    else {
        long double result = bin_pow(x, (pow - 1) / 2);
        return x * result * result;
    }
}

long double simple[SIZE];

void fill_simple()
{
    simple[0] = 0;
    simple[1] = 1;
    simple[2] = 2;
    simple[3] = 3;
    int pos = 4;
    for (int number = 4; number < SIZE; number++) {
        int i = 2;
        bool dividers = false;
        while (!dividers && i <= sqrt(number)) {
            if (number % i == 0) {
                dividers = true;
                break;
            }
            if (i < 3) {
                i++;
            }
            else {
                i += 2;
            }
        }
        if (!dividers) {
            simple[pos] = number;
            pos++;
        }
    }
}

//      limits      //

long double e_limit(char* argv) {
    long double eps = string_to_ld(argv);
    long long n = 2;
    long double fraction = ONE / n;

    while (fraction > eps) {
        n++;
        fraction = ONE / n;
    }

    return bin_pow((ONE + ONE / n), n);
}

long double pi_limit(char* argv) {
    long double eps = string_to_ld(argv);

    long long n = 1;
    long double pi = ONE;
    long double multiplier = ONE * 2;

    while (fabsl(pi - multiplier) > eps) {      // Формула Валлиса
        pi = multiplier;
        multiplier *= (ONE * 4 * n * n) / (ONE * 4 * n * n - 1);
        n++;
    }

    return pi;
}

long double ln2_limit(char* argv) {
    long double eps = string_to_ld(argv);

    long long n = 1;
    long double ln2 = ONE * 2;
    long double multiplier = ONE;

    while (fabsl(ln2 - multiplier) > eps) {
        ln2 = multiplier;
        n++;
        multiplier = n * (powl(ONE * 2, ONE / n) - 1);
    }

    return ln2;
}

long double sqrt2_limit(char* argv) {
    long double eps = string_to_ld(argv);

    long double x1 = 0L;
    long double x2 = -0.5L;

    while (fabsl(x2 - x1) > eps) {
        x1 = x2;
        x2 = x1 - (x1 * x1 / 2) + 1;
    }

    return x1;
}

long double gamma_limit(char* argv) {
    long double eps = string_to_ld(argv);

    long double result = 0L;
    long long n = 1;
    long double fraction = ONE;

    while(fraction > eps) {
        n++;
        result += fraction;
        fraction = ONE / n;
    }

    return result - logl(n);
}

//      tailor      //

long double e_tailor(char* argv) {
    long double eps = string_to_ld(argv);

    long long n = 1;
    long long factorial = 1;
    long double exponent = 0L;

    while (ONE / factorial > eps) {
        exponent += ONE / factorial;
        n++;
        factorial *= n;
    }

    return exponent + ONE;
}

long double pi_tailor(char* argv) {
    long double eps = string_to_ld(argv);

    long double pi = -ONE;
    long long n = 1;
    long double fraction = -ONE;
    long double pos_neg = ONE;

    while (fabsl(fraction) > eps) {
        n++;
        fraction = pos_neg / (ONE * 2 * n - 1);
        pi += fraction;
        pos_neg = -pos_neg;
    }

    return - ONE * 4 * pi;
}

long double ln2_tailor(char* argv) {
    long double eps = string_to_ld(argv);

    long double ln2 = ONE;
    long long n = 1;
    long double fraction = ONE;
    long double pos_neg = -ONE;

    while (fabsl(fraction) > eps) {
        n++;
        fraction = pos_neg / n;
        ln2 += fraction;
        pos_neg = -pos_neg;
    }

    return ln2;
}

long double sqrt2_tailor(char* argv) {
    long double eps = string_to_ld(argv);

    long double grade = ONE / (ONE * 2);
    long double sqrt2 = powl(2, grade);

    while(grade > eps) {
        sqrt2 *= powl(2, grade);
        grade *= 0.5L;
    }

    return sqrt2 / 2;
}

long double gamma_tailor(char* argv) {
    long double eps = string_to_ld(argv);

    long long koefs[100];
    for (int i = 0; i < 100; i++) {
        koefs[i] = i * i;
    }

    long double pi = atanl(1.0) * 4.0;
    long long k = 2;
    int n = 2;

    long double first_fraction = ONE;
    long double second_fraction = ONE / 2;
    long double gamma = 0.0L;

    while(fabsl(first_fraction - second_fraction) > eps) {
        if (k == koefs[n]) {
            k++;
            n++;
        }

        first_fraction = powl(floorl(sqrtl(k)), -2);
        second_fraction = ONE / k;
        gamma += first_fraction;
        gamma -= second_fraction;
        k++;
    }

    return gamma - ((pi * pi) / (ONE * 6));
}

//      equation        //

long double func_e(long double x) {
    return logl(x) - ONE;
}

long double df_e(long double x) {
    return 1.0 / x;
}

long double e_newton(char* argv) {
    long double eps = string_to_ld(argv);

    long double e = ONE * 2;
    long double fx = ONE * 3;

    while(fabsl(fx) > eps) {
        fx = func_e(e);
        long double dfx = df_e(e);
        e = e - fx / dfx;
    }

    return e;
}


long double func_pi(long double x) {
    return cosl(x) + ONE;
}

long double df_pi(long double x) {
    return -sinl(x);
}

long double pi_newton(char* argv) {
    long double eps = string_to_ld(argv);

    long double pi = 3 * ONE;
    long double fx = 4 * ONE;

    while (fabsl(fx) > eps) {
        fx = func_pi(pi);
        long double dfx = df_pi(pi);
        pi = pi - fx / dfx;
    }

    return pi;
}


long double func_ln2(long double x) {
    return expl(x) - 2.0L;
}

long double df_ln2(long double x) {
    return expl(x);
}

long double ln2_newton(char* argv) {
    long double eps = string_to_ld(argv);

    long double ln2 = ONE / 2;
    long double fx = ONE;

    while (fabsl(fx) > eps) {
        fx = func_ln2(ln2);
        long double dfx = df_ln2(ln2);
        ln2 = ln2 - fx / dfx;
    }

    return ln2;
}


long double func_sqrt2(long double x) {
    return x * x - 2;
}

long double df_sqrt2(long double x) {
    return 2 * x;
}

long double sqrt2_newton(char* argv) {
    long double eps = string_to_ld(argv);

    long double sqrt2 = ONE / 2;

    while (fabsl(func_sqrt2(sqrt2)) > eps) {
        sqrt2 = sqrt2 - func_sqrt2(sqrt2) / df_sqrt2(sqrt2);
    }

    return sqrt2;
}


void equalation_gamma_limit(char* argv, long double* limit) {
    long double eps = string_to_ld(argv);

    long long t = 2;
    long double result = ONE;
    long double up = 0;
    long double down = ONE;

    while (fabsl(up - down) > eps) {
        down = up;
        up = (simple[t] - ONE) / simple[t];
        result *= up;
        t++;
        if (t >= SIZE) {
            return;
        }
    }

    *limit = result * logl(simple[t]);
}

long double f_gamma(long double x, long double limit) {
    return expl(-x) - limit;
}

long double fd_gamma(long double x) {
    return -expl(x);
}

long double gamma_dirichle(char* argv) {
    long double eps = string_to_ld(argv);

    long double limit;
    equalation_gamma_limit(argv, &limit);

    long double gamma = 0.5L;
    long double x = gamma - (f_gamma(gamma, limit) / fd_gamma(gamma));
    int t = 0;

    while (fabsl(x - gamma) > eps) {
        gamma = x;
        x = gamma - (f_gamma(gamma, limit) / fd_gamma(gamma));
        t++;
        if (t > 1e9) {
            break;
        }
    }
    return gamma;
}
