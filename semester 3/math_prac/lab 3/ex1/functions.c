#include <stdio.h>
#include <stdlib.h>

// 2, 4, 8, 16, 32

const char symbol[32] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

int bit_sum(int a, int b) { // сложение через битовые операции
    if (a == b) {
        return a << 1;
    }

    int result, carry;
    result = a ^ b;
    carry = (a & b) << 1;
    while (carry) {
        int tmp = result;
        result ^= carry;
        carry = (tmp & carry) << 1;
    }
    return result;
}

int bit_subtr(int a, int b) { // вычитание b из a
    if (a == b) {
        return 0;
    }
    int negative = bit_sum(~b, 1);
    return bit_sum(a, negative);
}

int bit_sign(int num) {
    if (1 && (num >> 31)) {
        return 1;
    }
    else {
        return 0;
    }
}

int bit_abs(int num) { // модуль числа
    if (bit_sign(num)) {
        return bit_sum(~num, 1);
    }
    else {
        return num;
    }
}

int bit_multiply(int a, int b) { // умножение чисел
    if (a == 0 || b == 0) {
        return 0;
    }
    int is_Negative = 0;
    if (bit_sign(a) ^ bit_sign(b)) {
        is_Negative = 1;
    }

    a = bit_abs(a);
    b = bit_abs(b);
    int result = 0;
    while (b | 0) {
        if (b & 1) {
            result = bit_sum(result, a);
        }
        a <<= 1;
        b >>= 1;
    }

    if (is_Negative) {
        return bit_sum(~result, 1);
    }
    else {
        return result;
    }
}
/*
int bit_divide(int a, int b, int* flag) { // целочисленное деление числа 'a' на число 'b'
    if (b == 0) {
        *flag = 1;
        return 0;
    }
    if (a == 0) {
        return 0;
    }
    int is_Negative = 0;
    if (bit_sign(a) ^ bit_sign(b)) {
        is_Negative = 1;
    }

    a = bit_abs(a);
    b = bit_abs(b);
    int result = 0;
    while (a >= b) {
        result = bit_sum(result, 1);
        a = bit_subtr(a, b);
    }

    if (is_Negative) {
        return bit_sum(~result, 1);
    }
    else {
        return result;
    }
}
*/

int bit_2r_system(int num, int r) { // перевод в систему счисления 2^r
    if (r > 5 || r < 1) {
        printf("Unexpected grade of 2\n\n");
        return 0;
    }
    int base = 1;
    for (int i = 0; i < r; i = bit_sum(i, 1)) {
        base <<= 1;
    }
    base = bit_subtr(base, 1);

    int max_length = 0;
    for (int i = bit_abs(num); i > 0; i >>= r) {
        max_length = bit_sum(max_length, 1);
    }

    char* result = (char*)malloc(bit_multiply(sizeof(char), max_length));
    int index = 0;
    for (int i = bit_abs(num); i > 0; i >>= r)
    {
        result[index] = symbol[i & base];
        index = bit_sum(index, 1);
    }

    base = bit_sum(base, 1);
    printf("In base %d number %d equals: ", base, num);
    if (num < 0) {
        printf("-");
    }
    for (int i = 1; i <= index; i = bit_sum(i, 1)) {
        printf("%c", result[bit_subtr(index, i)]);
    }
    printf("\n\n");
    free(result);
    return 0;
}
