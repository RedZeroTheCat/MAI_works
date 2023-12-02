#include <stdio.h>

#include "functions.c"

int main()  {
    // ex2.1 - среднее геометрическое введённых чисел (переменное число аргументов)
    long double result = average_geometric(6, 1.0, 3.0, 6.0, 7.0, 9.0, 2.0);
    if (result == -2) {
        printf("Something incorrect with input, possibly - negative num\n");
    }
    else if (result == -1) {
        printf("Negative count of numbers\n");
    }
    else {
        printf("%Lf \n", result);
    }

    result = average_geometric(5, 1.0, 3.0, -6.0, -7.0, 9.0, 2.0);
    if (result == -2) {
        printf("Something incorrect with input, possibly - negative num\n");
    }
    else if (result == -1) {
        printf("Negative count of numbers\n");
    }
    else {
        printf("%Lf \n", result);
    }

    result = average_geometric(6, 2.0, 3.0, 6.0, -7.0, 9.0, 2.0);
    if (result == -2) {
        printf("Something incorrect with input, possibly - negative num\n");
    }
    else if (result == -1) {
        printf("Negative count of numbers\n");
    }
    else {
        printf("%Lf \n", result);
    }

    result = average_geometric(-6, 1.0, 3.0, 6.0, 7.0, 9.0, 2.0);
    if (result == -2) {
        printf("Something incorrect with input, possibly - negative num\n");
    }
    else if (result == -1) {
        printf("Negative count of numbers\n");
    }
    else {
        printf("%Lf \n", result);
    }

    printf("\n=====================\n\n");

    // ex2.2 - рекурсивная функция возведения вещественного числа в целую степень
    printf("%Lf \n", bin_pow(6.9, 2));

    printf("%Lf \n", bin_pow(0, 6));

    printf("%Lf \n", bin_pow(-1, 6));
    printf("%Lf \n", bin_pow(-1, 5));

    printf("%Lf \n", bin_pow(2, -2));

    printf("%Lf \n", bin_pow(199999, 100));
    printf("%Lf \n", bin_pow(199999, 10000000));
}
