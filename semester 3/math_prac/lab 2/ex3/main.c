#include <stdio.h>
#include <stdlib.h>

#include "functions.c"

int main() {
    CELL** cells;
    int count = 2;
    char* pattern = "hello";
    status_code state = find_all_patterns(&cells, pattern, count, "in1.txt", "in2.txt");
    switch (state) {
        case sc_correct:
            for (int i = 0; i < count; i++) {
                print_CELL(cells[i]);
            }
            free(cells);
            break;
        case sc_file_error:
            printf("Something wrong with files\n");
            break;
        case sc_memory_error:
            printf("Memory error\n");
            break;
    }
    return 0;
}
