#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    fill_simple();

    switch (analyzer(argc, argv)) {
        case isc_correct_input:
            printf("================  E  ================\n");
            printf("By limit:\t\t%.9Lf\n", e_limit(argv[1]));
            printf("By Tailor:\t\t%.9Lf\n", e_tailor(argv[1]));
            printf("By equalation:\t\t%.9Lf\n", e_newton(argv[1]));
            printf("================[^^^]================\n\n");

            printf("===============  PI  ================\n");
            printf("By limit:\t\t%.9Lf\n", pi_limit(argv[1]));
            printf("By Tailor:\t\t%.9Lf\n", pi_tailor(argv[1]));
            printf("By equalation:\t\t%.9Lf\n", pi_newton(argv[1]));
            printf("===============[^^^^]================\n\n");

            printf("=============== ln(2) ===============\n");
            printf("By limit:\t\t%.9Lf\n", ln2_limit(argv[1]));
            printf("By Tailor:\t\t%.9Lf\n", ln2_tailor(argv[1]));
            printf("By equalation:\t\t%.9Lf\n", ln2_newton(argv[1]));
            printf("===============[^^^^^]===============\n\n");

            printf("============== sqrt(2) ==============\n");
            printf("By limit:\t\t%.9Lf\n", sqrt2_limit(argv[1]));
            printf("By Tailor:\t\t%.9Lf\n", sqrt2_tailor(argv[1]));
            printf("By equalation:\t\t%.9Lf\n", sqrt2_newton(argv[1]));
            printf("==============[^^^^^^^]==============\n\n");

            printf("=============== gamma ===============\n");
            printf("By limit:\t\t%.9Lf\n", gamma_limit(argv[1]));
            printf("By Tailor:\t\t%.9Lf\n", gamma_tailor(argv[1]));
            printf("By equalation:\t\t%.9Lf\n", gamma_dirichle(argv[1]));
            printf("===============[^^^^^]===============\n\n");
            break;

        case isc_wrong_input:
            printf("Invalid input, please check, fix and try again.\n");
            break;
    }
    return 0;
}
