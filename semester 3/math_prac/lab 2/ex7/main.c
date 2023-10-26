#include <math.h>
#include <stdio.h>

#include "functions.c"

#define _USE_MATH_DEFINES

int main()
{
    double eps = 0.00001;

    double d_func1 = dichotomy(1, 6.5, &func_1, eps);
    double d_func2 = dichotomy(0, 8, &func_2, eps);
    double d_func3 = dichotomy(-M_PI, M_PI/2, &func_3, eps);
    double d_func4 = dichotomy(0.125, 0.5, &func_4, eps);

    printf("Calculated X in (x^2 - 2.25) equals: %.6f\n", d_func1);
    printf("X in equation = %.6f\n\n", func_1(d_func1));

    printf("Calculated X in e^x equals:  %.6f\n", d_func2);
    printf("X in equation = %.6f\n\n", func_2(d_func2));

    printf("Calculated X in cos(X) equals:  %.6f\n", d_func3);
    printf("X in equation = %.6f\n\n", func_3(d_func3));

    printf("Calculated X in log2(X) equals:  %.6f\n", d_func4);
    printf("X in equation = %.6f\n\n", func_4(d_func4));
}
