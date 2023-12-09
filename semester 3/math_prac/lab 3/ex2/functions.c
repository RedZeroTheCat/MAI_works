#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef struct {
    double* value;
    int size;
} VECTOR;

typedef double (*norm_ptr)(VECTOR*, void*);

void print_vector(VECTOR* v) {
    printf("{");
    for (int i = 0; i < v->size; i++) {
        printf("%.2f", v->value[i]);
        if (i != v->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

void delete_vector(VECTOR* v) {
    free(v->value);
}

VECTOR get_random_vec(int size) {
    VECTOR v;
    double* cords = (double*)malloc(sizeof(double) * size);
    if (cords == NULL) {
        return v;
    }
    for (int i = 0; i < size; i++) {
        cords[i] = (rand() % 10) * 1.0;
    }
    v.size = size;
    v.value = cords;
    return v;
}

double norm_first(VECTOR* vector, void* param) {
    double max_cord = 0;
    for (int i = 0; i < vector->size; i++) {
        double cord = fabs(vector->value[i]);
        if (cord > max_cord) {
            max_cord = cord;
        }
    }
    return max_cord;
}

double norm_second(VECTOR* vector, void* param) {
    double p = *(double*)param;
    double sum = 0;
    for (int i = 0; i < vector->size; i++) {
        sum += pow(fabs(vector->value[i]), p);
    }
    return pow(sum, 1.0 / p);
}

double norm_third(VECTOR* vector, void* param) {
    double* matrix = (double*)param;
    int n = vector->size;
    double sum = 0;
    for (int i = 0; i < n; i++) {
        double tmp = 0;
        for (int j = 0; j < n; j++) {
            tmp += vector->value[j] * matrix[i * n + j];
        }
        sum += (fabs(tmp) * vector->value[i]);
    }
    return sqrt(sum);
}

VECTOR** get_norm(int cnt_vectors, int cnt_norms, ...) {
    VECTOR** result = (VECTOR**)malloc(cnt_norms * sizeof(VECTOR*));
    if (result == NULL) {
        return NULL;
    }

    VECTOR* vectors = (VECTOR*)malloc(cnt_vectors * sizeof(VECTOR));
    if (vectors == NULL) {
        free(result);
        return NULL;
    }

    va_list args;
    va_start(args, cnt_norms);
    for (int i = 0; i < cnt_vectors; i++) {
        vectors[i] = va_arg(args, VECTOR);
    }

    for (int i = 0; i < cnt_norms; i++) {
        VECTOR* tmp = (VECTOR*)malloc((cnt_vectors + 1) * sizeof(VECTOR));
        if (tmp == NULL) {
            free(vectors);
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        norm_ptr func = va_arg(args, norm_ptr);
        void* param = va_arg(args, void*);

        double max_norm = -1;
        int count = 0;
        for (int j = 0; j < cnt_vectors; j++) {
            VECTOR v = vectors[j];
            double norm = func(&v, param);
            if (norm > max_norm) {
                max_norm = norm;
                count = 0;
            }
            if (norm == max_norm) {
                tmp[count] = v;
                count++;
            }
        }
        tmp[count].value = NULL;
        VECTOR* ptr = realloc(tmp, (count + 1) * sizeof(VECTOR));
        if (ptr == NULL) {
            free(vectors);
            free(tmp);
            for (int j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            va_end(args);
            return NULL;
        }
        result[i] = ptr;
    }
    va_end(args);
    free(vectors);
    return result;
}
