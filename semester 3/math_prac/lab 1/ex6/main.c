#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    ld eps = 0;
    switch (check_input(argc, argv, &eps)) {
        case sc_invalid:
            printf("ERROR: incorrect request, fix and try again\n");
            return 1;
        case sc_wrong_argument:
            printf("ERROR: incorrect epsilon input, fix and try again\n");
            return 1;
        case sc_correct:
            printf("Calculation for integral A: %.9Lf\n",
                    mid_point(eps, 0.0L, 1.0L, &integral_a));
            printf("Calculation for integral B: %.9Lf\n",
                    mid_point(eps, 0.0L, 1.0L, &integral_b));
            printf("Calculation for integral C: %.9Lf\n",
                    mid_point(eps, 0.0L, 1.0L, &integral_c));
            printf("Calculation for integral D: %.9Lf\n",
                    mid_point(eps, 0.0L, 1.0L, &integral_d));
    }
    return 0;
}
