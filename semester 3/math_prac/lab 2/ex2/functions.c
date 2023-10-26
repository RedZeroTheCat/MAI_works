#include <stdio.h>
#include <stdarg.h>
#include <math.h>

long double average_geometric(int count, ...) {
    if (count == 0) {
        return 0;
    }
    va_list argv;
    va_start(argv, count);
    long double result = 1.0L;

    for (int i = 0; i < count - 1; i++) {
        double arg = va_arg(argv, double);
        result *= arg;
    }
    result = powl(result, (1.0 / count));
    va_end(argv);
    return result;
}

long double bin_pow(long double num, long long pow)
{
    if (pow < 0) {
        pow *= -1;
        long double result = bin_pow(num, pow);
        return 1.0/result;
    }
    else {
        if (pow == 1) {
            return num;
        }
        if (pow == 0) {
            return 1;
        }
        if (pow % 2 == 0) {
            long double result = bin_pow(num, pow / 2);
            return result * result;
        }
        else {
            long double result = bin_pow(num, (pow - 1) / 2);
            return num * result * result;
        }
    }
    return 0;
}
