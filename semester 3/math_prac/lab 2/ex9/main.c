#include <stdio.h>

#include "functions.c"

int main() {
    representation_check(16, 6, 0.5, 0.25, 0.125, 0.1, 0.2, 0.3);
    printf("\n");
    representation_check(10, 6, 0.5, 0.00225, 0.125, 0.1, 0.2, 0.3);
    printf("\n");
    representation_check(8, 6, 0.5, 0.25, 0.125, 0.1, 0.2, 0.3);
    printf("\n");
    representation_check(5, 6, 0.5, 0.25, 0.125, 0.1, 0.2, 0.3);
    printf("\n");
    representation_check(3, 6, 0.5, 0.25, 0.125, 0.1, 0.2, 0.3);
    printf("\n");
    representation_check(2, 6, 0.5, 0.25, 0.125, 0.1, 0.2, 0.3);
    return 0;
}
