#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_BUFF 1024

typedef struct {
    int id;
    char name[STR_BUFF];
    char surname[STR_BUFF];
    double money;
} Employee;

typedef enum {
    correct,
    memory_error,
    invalid_file,
    invalid_input,
    end_of_valid
} status_code;

void print_error(status_code state) {
    switch (state) {
        case memory_error:
            printf("Memory error\n");
            break;
        case invalid_file:
            printf("Unable to open file\n");
            break;
        case invalid_input:
            printf("Invalid input\n");
            break;
        case end_of_valid:
            printf("End of correct data\n");
            break;
        case correct:
            break;
    }
}

void print_employers(Employee* emps, int count) {
    printf("\n");
    for (int i = 0; i < count; i++) {
        printf("Earnings: %.lf, Surname: %s, Name: %s, ID: %d\n",
            emps[i].money, emps[i].surname, emps[i].name, emps[i].id);
    }
    printf("\n");
}

int cmp_increase(const void* a, const void* b) {
    Employee* first = (Employee*)a;
    Employee* second = (Employee*)b;
    int result;

    result = first->money - second->money;
    if (result != 0) {
        return result;
    }

    result = strcmp(first->surname, second->surname);
    if (result != 0) {
        return result;
    }

    result = strcmp(first->name, second->name);
    if (result != 0) {
        return result;

    }
    result = first->id - second->id;
    return result;
}

int cmp_decrease(const void* a, const void* b) {
    return cmp_increase(b, a);
}

status_code sort_by_increase(Employee** employers, int count) {
    if (count < 1) {
        return invalid_input;
    }
    qsort(*employers, count, sizeof((*employers)[0]), cmp_increase);
    return correct;
}

status_code sort_by_decrease(Employee** employers, int count) {
    if (count < 1) {
        return invalid_input;
    }
    qsort(*employers, count, sizeof((*employers)[0]), cmp_decrease);
    return correct;
}

status_code validate(int argc, char* argv[], FILE** input, char* flag, FILE** output) {
    if (argc < 3 || strlen(argv[2]) != 2) {
        return invalid_input;
    }
    char a = argv[2][0];
    if (a != '/' && a != '-') {
        return invalid_input;
    }
    char b = argv[2][1];
    if (b != 'a' && b != 'd') {
        return invalid_input;
    }

    *flag = b;
    *input = fopen(argv[1], "r");
    if (*input == NULL) {
        return invalid_file;
    }
    *output = fopen(argv[3], "w");
    if (*output == NULL) {
        fclose(*input);
        return invalid_file;
    }
    return correct;
}

status_code scan_employer(FILE* input, Employee* emp) {
    int id;
    char name[STR_BUFF];
    char surname[STR_BUFF];
    double money;

    if (fscanf(input, "%d %s %s %lf\n", &id, name, surname, &money) != 4) {
        return end_of_valid;
    }
    emp->id = id;
    strcpy(emp->name, name);
    strcpy(emp->surname, surname);
    emp->money = money;
    return correct;
}

void fprint_employers(Employee* emps, int count, FILE** output) {
    fprintf(*output,"\n");
    for (int i = 0; i < count; i++) {
        fprintf(*output, "Earnings: %.lf, Surname: %s, Name: %s, ID: %d\n",
            emps[i].money, emps[i].surname, emps[i].name, emps[i].id);
    }
    fprintf(*output, "\n");
}
