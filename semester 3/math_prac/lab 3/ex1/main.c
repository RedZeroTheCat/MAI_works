#include <stdio.h>

#include "functions.c"

int main() {
    bit_2r_system(3111, 0);
    bit_2r_system(-3111, 1);
    bit_2r_system(3111, 2);
    bit_2r_system(-3111, 3);
    bit_2r_system(3111, 4);
    bit_2r_system(-3111, 5);
    bit_2r_system(3111, 6);
    return 0;
}
