#include <stdio.h>
#include <math.h>

double func_1(double x) {
    return x * x - 2.25;
}

double func_2(double x) {
    return exp(x);
}

double func_3(double x) {
    return cos(x);
}

double func_4(double x) {
    return log2(x);
}

double dichotomy(double start, double end, double (*func)(double), double eps)
{
    double mid = 0;
    while (fabs(end - start) > fabs(eps)) {
        mid = (start + end) / 2;

        if (func(mid) >= -fabs(eps) && func(mid) <= fabs(eps)) {
            return mid;
        }
        if (func(mid) * func(start) < fabs(eps)) {
            end = mid;
        }
        else {
            start = mid;
        }
    }
    return mid;
}
