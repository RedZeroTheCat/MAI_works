#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    sc_correct,
    sc_memory_error
} status_code;

void print_array(long array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%ld ", array[i]);
    }
    printf("\n");
}

/////////////////////[ 9.1 ]/////////////////////
void fill_array(long array[], int size, long a, long b) {
    for (int i = 0; i < size; i++) {
        array[i] = a + rand() % (b - a + 1);
    }
}

void swap_min_max(long array[], int size) {
    if (size <= 1) {
        return;
    }

    int index_min = 0;
    int index_max = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] < array[index_min]) {
            index_min = i;
        }
        if (array[i] > array[index_max]) {
            index_max = i;
        }
    }
    long tmp = array[index_min];
    array[index_min] = array[index_max];
    array[index_max] = tmp;
}

/////////////////////[ 9.2 ]/////////////////////
status_code generate_array(long** array, int size, int a, int b) {
    *array = (long*)malloc(sizeof(long) * size);
    if (*array == NULL) {
        return sc_memory_error;
    }
    for (int i = 0; i < size; i++) {
        (*array)[i] = rand() % (b - a + 1) + a;
    }
    return sc_correct;
}

long find_closest(long value, long* array, int size) {
    long closest_to_value = array[0];
    long min_diff = labs(value - array[0]);

    for (int i = 1; i < size; i++) {
        long diff = labs(value - array[i]);
        if (diff < min_diff) {
            min_diff = diff;
            closest_to_value = array[i];
        }
    }
    return closest_to_value;
}

status_code combine_arrays(long** array_c, long* array_a, long* array_b, int size_a, int size_b) {
    *array_c = (long*)malloc(sizeof(long) * size_a);
    if (*array_c == NULL) {
        return sc_memory_error;
    }
    for (int i = 0; i < size_a; i++) {
        long closest_value = find_closest(array_a[i], array_b, size_b);
        (*array_c)[i] = array_a[i] + closest_value;
    }
    return sc_correct;
}
