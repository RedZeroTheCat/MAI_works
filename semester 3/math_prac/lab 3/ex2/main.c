#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "functions.c"

int main() {
    srand(time(NULL));
    VECTOR v1 = get_random_vec(3);
    VECTOR v2 = get_random_vec(3);
    VECTOR v3 = get_random_vec(3);
    print_vector(&v1);
    print_vector(&v2);
    print_vector(&v3);

    double p = 2.0;
    double A[] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
        };

    VECTOR** result = get_norm(3, 3, v1, v2, v3, &norm_first, NULL, &norm_second, &p, &norm_third, A);
    if (result == NULL) {
        printf("Something went wrong\n");
        delete_vector(&v1);
        delete_vector(&v2);
        delete_vector(&v3);
        return 1;
    }

    for (int i = 0; i < 3; i++) {
        printf("For norm function %d:\n", i + 1);
        for (int j = 0; result[i][j].value != NULL; j++) {
            printf("Vector: ");
            print_vector(&(result[i][j]));
        }
        free(result[i]);
    }
    delete_vector(&v1);
    delete_vector(&v2);
    delete_vector(&v3);

    // VECTOR x;
    // x.size = 2;
    // double i[] = {1.0, 2.0};
    // x.value = i;
    // double m[] = {1, 2, 3, 4};
    // printf("\n%f\n", norm_third(&x, m));
    return 0;
}
