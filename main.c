#include <stdio.h>
#include <math.h>

#include "enum.h"
#include "functions.c"

int main(int argc, char** argv) 
{
    int string_length = 0;
    unsigned long long factorial_result;
    switch(analyzer(argc, argv)) {
        case isc_invalid_argument:
            printf("Incorrect input.\n");
            break;
        case isc_asking_for_help:
            printf("Possible flags:\n"
            "-h or /h output to the console natural numbers within 100 inclusive, multiples of the specified.\n"
            "-p or /p determines whether the entered number is simple or composite.\n"
            "-s or /s divides the number into separate digits and outputs each digit of the number separately,\n"
            "separating them with a space, from most significant to least significant digits,\n" 
            "without leading zeros in the string representation.\n"
            "-e or /e displays a table of degrees (for all indicators in the range from 1 to the specified number)\n"
            "bases from 1 to 10; For this flag there is a restriction on the number entered: it should not be more than 10.\n"
            "-a or /a calculates the sum of all natural numbers from 1 to a specified number inclusive\n"
            "and outputs the resulting value to the console.\n"
            "-f or /f calculates the factorial of the specified number and outputs the resulting value to the console.\n"
            );
            break;
        case isc_no_number_input:
            printf("Number was not entered.\n");
            break;
        case isc_negative_number_input:
            printf("You have entered negative number.\n");
            break;
        case isc_too_many_arguments:
            printf("You have entered too many argument.\n");
            break;
        case isc_h:
            natural(argv[2]);
            break;
        case isc_p:
            switch(is_simple(argv[2])) {
                case psc_negative:
                    printf("You have entered negative number, please try again.\n");
                    break;
                case psc_zero:
                    printf("You have entered 0, please try again.\n");
                    break;
                case psc_one:
                    printf("You have entered 1, which is simple.\n");
                    break;
                case psc_complicated:
                    printf("The entered number is complicated.\n");
                    break;
                case psc_simple:
                    printf("The entered number is simple.\n");
                    break;
            }
            break;
        case isc_s:
            string_length = strlen(argv[2]);
            char* result = malloc(sizeof(char)*2*string_length);
            switch (divided_string(argv[2], result)) {
                case ssc_correct_input:
                    printf("Divided entered string: %s \n", result);
                    break;
                case ssc_wrong_input:
                    printf("Your input is incorrect, please try again.\n");
                    break;
            }
            free(result);
            break;
        case isc_e:
            switch(table_of_grades(argv[2])) {
                case esc_correct_input:
                    grade_table(argv[2]);
                    break;
                case esc_out_of_bounds:
                    printf("Invalid number, please try again.\n");
            }
            break;
        case isc_a:
            switch(arithmetic_sum(argv[2])) {
                case asc_correct_input:
                    printf("Sum of naturals up to entered number is: %lld \n", alg_sum(argv[2]));
                    break;
                case asc_negative:
                    printf("You have entered negative number, please try again.\n");
                case asc_zero:
                    printf("You have entered 0, please try again.\n");
            }
            break;
        case isc_f:
            switch(factorial(string_to_ll(argv[2]), &factorial_result)) {
                case fsc_ok:
                    printf("Factorial of entered number: %u! = ", string_to_ll(argv[2]));
                    if (string_to_ll(argv[2]) < 0) {
                        printf("-");
                    }
                    printf("%llu \n", factorial_result);
                    break;

                case fsc_invalid_parameter:
                    printf("Invalid parameter.\n");
                    break;
            }
            break;
    }
}