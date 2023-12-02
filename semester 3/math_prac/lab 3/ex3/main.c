#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    FILE* input;
    FILE* output;
    char flag;
    status_code state = validate(argc, argv, &input, &flag, &output);
    if (state != correct) {
        print_error(state);
        return 0;
    }

    int buff = 2;
    int count = 0;
    Employee* employers = (Employee*)malloc(sizeof(Employee) * buff);
    if (employers == NULL) {
        fclose(input);
        fclose(output);
        printf("Memory error\n");
        return 0;
    }

    Employee emp;
    while ((state = scan_employer(input, &emp)) == correct) {
        //printf("Employer %d %s %s %lf\n", emp.id, emp.name, emp.surname, emp.money);
        if (buff <= count) {
            buff *= 2;
            Employee* new_employers = (Employee*)realloc(employers, sizeof(Employee) * buff);
            if (new_employers == NULL) {
                free(employers);
                fclose(input);
                fclose(output);
                printf("Memory error\n");
                return 0;
            }
            employers = new_employers;
        }
        employers[count] = emp;
        count++;
    }
    fclose(input);
    print_error(state);

    if (flag == 'd') {
        sort_by_decrease(&employers, count);
    }
    else {
        sort_by_increase(&employers, count);
    }
    fprint_employers(employers, count, &output);
    fclose(output);
    free(employers);
    return 0;
}
