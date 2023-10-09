#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "enum.h"

bool is_num(char* str) {
    char* terminator;
    long num = strtol(str, &terminator, 10);

    if (*terminator != '\0') {
        return false;
    }
    return true;
}

long long string_to_ll(char* argv) {
    long long result = 0;
    int string_length = strlen(argv);
    if (argv[0] == '-') {                // Если введённое число отрицательное
        for (int i = 1; i < string_length; i++) { 
            result *= 10;
            result += argv[i] - '0';
        }
        result *= -1;
    }
    else {                              // Если введённое число положительное
        for (int i = 0; i < string_length; i++) {
            result *= 10;
            result += argv[i] - '0';
        }
    }
    return result;
}

bool compare_flag(char** argv) {
    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "/h")) {
        return true;
    }
    else if (!strcmp(argv[1], "-p") || !strcmp(argv[1], "/p")) {
        return true;
    }
    else if (!strcmp(argv[1], "-s") || !strcmp(argv[1], "/s")) {
        return true;
    }
    else if (!strcmp(argv[1], "-e") || !strcmp(argv[1], "/e")) {
        return true;
    }
    else if (!strcmp(argv[1], "-a") || !strcmp(argv[1], "/a")) {
        return true;
    }
    else if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "/f")) {
        return true;
    }
    else {
        return false;
    }
}

enum input_status_code analyzer(int argc, char** argv) {
    if (argc == 2 && (!strcmp(argv[1], "-help") || !strcmp(argv[1], "/help"))) {
        return isc_asking_for_help;
    }
    else if (argc == 2) {
        if (compare_flag(argv)) {
            return isc_no_number_input;
        }
        else {
            return isc_invalid_argument;
        }
    }
    else if (argv[2][0] == '-' && (argv[2][1] > '0' || argv[2][1] < '9')) {
        return isc_negative_number_input;
    }
    else if (atoi(argv[2]) == 0 && strlen(argv[2]) > 1) {
        return isc_invalid_argument;
    }
    else if (!is_num(argv[2])) {
        return isc_invalid_argument;
    }
    else if (argc > 3) {
        return isc_too_many_arguments;
    }
    else if (argv[2][0] < '0' || argv[2][0] > '9') {
        return isc_invalid_argument;
    }
    else if (argc == 3) {
        if ((!strcmp(argv[1], "-h") || !strcmp(argv[1], "/h")) && (strtol(argv[2], NULL, 10) != 0)) {
            return isc_h;
        }
        else if (!strcmp(argv[1], "-p") || !strcmp(argv[1], "/p")) {
            return isc_p;
        }
        else if (!strcmp(argv[1], "-s") || !strcmp(argv[1], "/s")) {
            return isc_s;
        }
        else if (!strcmp(argv[1], "-e") || !strcmp(argv[1], "/e")) {
            return isc_e;
        }
        else if (!strcmp(argv[1], "-a") || !strcmp(argv[1], "/a")) {
            return isc_a;
        }
        else if (!strcmp(argv[1], "-f") || !strcmp(argv[1], "/f")) {
            return isc_f;
        }
        else {
            return isc_invalid_argument;
        }
    }
}


void natural(char* argv)        // натуральные числа в пределах 100 включительно, кратные указанному
{
    int entered_number = string_to_ll(argv);
    if (entered_number > 100) {
        printf("There are no numbers up to 100 that are multiples of the entered number.\n");
    }
    else {
        printf("Numbers that are multiples of the entered:\n");
        for (int i = entered_number; i < 101; i += entered_number) {
            printf("%d ", i);
        }
    }
} 

enum plain_status_code is_simple(char* argv)         // является ли введенное число простым или составным
{
    long long number = string_to_ll(argv);
    if (number < 0) {
        return psc_negative;
    }
    else if (number == 0) {
        return psc_zero;
    }
    else if (number == 1) {
        return psc_one;
    }
    else {
        int i = 2;
        bool dividers = false;
        while (!dividers && i <= sqrt(number)) {
            if (number % i == 0) {
                dividers = true;
                break;
            } 
            if (i < 3) {
                i++;
            }
            else {
                i += 2;
            }
        }
        if (dividers) {
            return psc_complicated;
        }
    }
    return psc_simple;
}

enum string_status_code divided_string(char* argv, char* result)      //  разделить число на отдельные цифры и вывести отдельно каждую цифру числа
{
    int string_length = strlen(argv);
    int worth_number = 0;
    int rp = 0;
    int i = 0;
    if (argv[0] == '-') {
        result[0] = '-';
        result[1] = ' ';
        i++;
        rp += 2;
    }
    while (i < string_length) {
        if (!(argv[i] >= 48 && argv[i] <= 57)) {
            return ssc_wrong_input;
        }
        // if (argv[i] == '0' && !worth_number) {
        //     i++;
        //     continue;
        // }
        if (argv[i] != '0') {
            worth_number++;
            result[rp] = argv[i];
            rp++;
            result[rp] = ' ';
            rp++;
        }
        if (argv[i] == '0' && worth_number) {
            result[rp] = argv[i];
            rp++;
            result[rp] = ' ';
            rp++;
        }
        i++;
    }
    result[rp] = '\0';
    return ssc_correct_input;
}

void grade_table(char* argv)    // вывести таблицу степеней оснований от 1 до 10
{
    int number = string_to_ll(argv);
    for (int i = 0; i < 22; i++) {
        printf("=====");
    }
    printf("\n");
    int current = 1;
    while (current < number + 1) {
        long long tmp = current;
        for (int i = 1; i < 11; i++) {
            if (i < 4) {
                printf("|%6llu ", tmp);
            }
            else {
                printf("| %10llu", tmp);
            }
            tmp *= current;
        }
        printf("|\n");
        for (int i = 0; i < 22; i++) {
            printf("=====");
        }
        printf("\n");
        current++;
    }
}

enum extent_status_code table_of_grades(char* argv) 
{
    int number = string_to_ll(argv);
    if (number > 10 || number < 1) {
        return esc_out_of_bounds;
    }
    else {
        return esc_correct_input;
    }
}

long long alg_sum(char* argv)        // вычислить сумму всех натуральных чисел от 1 до указанного
{
    long long number = string_to_ll(argv);
    long double answer_double = number*(0.5*(number + 1));
    long long answer = answer_double;
    return answer;
}

enum algebra_status_code arithmetic_sum(char* argv) {
    long long number = string_to_ll(argv);
    if (number == 0) {
        return asc_zero;
    }
    else if (number < 0) {
        return asc_negative;
    }
    else {
        return asc_correct_input;
    }
}

enum factorial_status_code factorial(unsigned int value, unsigned long long* result)      // вычислить факториал указанного числа 
{
    enum factorial_status_code recursive_status_code;

    if (value > 20) {
        return fsc_invalid_parameter;
    }

    if (value == 0 || value == 1) {
        *result = 1;
        return fsc_ok;
    }

    recursive_status_code = factorial(value - 1, result);
    if (recursive_status_code == fsc_ok) {
        *result *= value;
    }

    return recursive_status_code;
}


