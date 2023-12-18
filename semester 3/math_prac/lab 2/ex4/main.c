#include <asm-generic/errno.h>
#include <stdbool.h>
#include <stdio.h>

#include "functions.c"

int main() {
    struct cord v = {31, 42};

    struct cord v01 = {3, -8};
    struct cord v02 = {9, 7};

    struct cord v11 = {0, 0};
    struct cord v12 = {1, 0};
    struct cord v13 = {0, 2};

    struct cord v21 = {-2, 1};
    struct cord v22 = {0, 1};
    struct cord v23 = {1, 1};
    struct cord v24 = {2, 1};

    struct cord v31 = {-1, 1};
    struct cord v32 = {2, 4};
    struct cord v33 = {3, 3};
    struct cord v34 = {4, 1};
    struct cord v35 = {3, -1};
    struct cord v36 = {1, -2};

    analyzer(is_convex(1, v));
    printf("\n");
    analyzer(is_convex(2, v01, v02));
    printf("\n");
    analyzer(is_convex(3, v11, v12, v13));
    printf("\n");
    analyzer(is_convex(4, v21, v22, v23, v24));
    printf("\n");
    analyzer(is_convex(6, v31, v32, v33, v34, v35, v36));
    printf("\n");

    bool pos_grade = true;          // 2*x^2 + x + 3 with x = 2
    double result = polynom_value(2.0, &pos_grade, 2, 2.0, 1.0, 3.0);
    if (pos_grade) {
        printf("Polynom equals: %lf\n", result);
    }
    else {
        printf("Negative grade of polynom entered\n");
    }

    pos_grade = true;               // x^3 + 2x^2 + x - 1 with x = 3
    result = polynom_value(3.0, &pos_grade, 3, 0.0, 2.0, 1.0, -1.0);
    if (pos_grade) {
        printf("Polynom equals: %lf\n", result);
    }
    else {
        printf("Negative grade of polynom entered\n");
    }
}
