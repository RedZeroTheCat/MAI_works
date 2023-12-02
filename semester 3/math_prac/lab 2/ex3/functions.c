#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h> // только для length
#include <stdbool.h>

typedef enum {
    sc_correct,
    sc_memory_error,
    sc_file_error,
    sc_everything
} status_code;

typedef struct {
    int* string_pos;
    int* char_pos;
    int count;
    char* path;
    status_code state;
} CELL;

void free_CELL(CELL* cell) {
    if (cell == NULL) {
        return;
    }
    free(cell -> char_pos);
    free(cell -> string_pos);
    free(cell);
    cell = NULL;
}

void free_CELLs(CELL** cells, int count) {
    if (cells == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free_CELL(cells[i]);
    }
}

void print_CELL(CELL* cell) {
    if (cell == NULL) {
        return;
    }
    printf("\n");
    printf("For file: %s\n", cell -> path);
    if (cell -> state == sc_everything) {
        int string = 1;
        int pos = 1;
        char cur;
        FILE* input = fopen(cell -> path, "r");
        if (input == NULL) {
            return;
        }
        while ((cur = fgetc(input)) != EOF) {
            printf("In line %d at column %d\n", string, pos);
            if (cur == '\n') {
                string++;
                pos = 0;
            }
            pos++;
        }
        return;
    }
    if (cell -> state == sc_correct) {
        printf("Count is: %d\n", cell -> count);
        for (int i = 0; i < cell -> count; i++) {
            printf("In line %d at column %d\n", cell->string_pos[i], cell->char_pos[i]);
        }
    }
    printf("\n");
}

bool is_equal_string(char* pattern, char* string, int patt_len) {
    for (int i = 0; i < patt_len; i++) {
        if (pattern[i] != string[i]) {
            return false;
        }
    }
    return true;
}

void move_char_left(char* string, int length) {
    if (length == 1) {
        return;
    }
    for (int i = 0; i < length - 1; i++) {
        string[i] = string[i + 1];
    }
}

void move_int_left(int* string, int length) {
    if (length == 1) {
        return;
    }
    for (int i = 0; i < length - 1; i++) {
        string[i] = string[i + 1];
    }
}

status_code find_pattern(char* pattern, char* buffer, int patt_len, FILE* input, CELL** result) {
    if (patt_len == 0) {
        CELL* cell = (CELL*)malloc(sizeof(CELL));
        cell -> state = sc_everything;
        *result = cell;
        return sc_correct;
    }

    size_t bytes_read = 0;
    int buffer_size = 2;
    int* mas = (int*)malloc(sizeof(int) * patt_len + 1);
    if (mas == NULL) {
        return sc_memory_error;
    }
    int* str_i = (int*)malloc(sizeof(int) * buffer_size);
    if (str_i == NULL) {
        free(mas);
        return sc_memory_error;
    }
    int* ind_i = (int*)malloc(sizeof(int) * buffer_size);
    if (ind_i == NULL) {
        free(mas);
        free(str_i);
        return sc_memory_error;
    }

    char symbol;
    int string_n = 1;
    int char_n = 1;
    int fake_newline = 0;
    for (int i = 0; i < patt_len; i++) {
        if (pattern[i] == '\n') {
            fake_newline++;
        }
    }

    mas[0] = 1;
    while (bytes_read != patt_len && (symbol = fgetc(input)) != EOF) {
        char_n++;
        if (symbol =='\n') {
            mas[bytes_read] = 1;
            string_n++;
            char_n = 1;
        }
        else if (bytes_read > 0) {
            mas[bytes_read] = mas[bytes_read - 1] + 1;
        }
        buffer[bytes_read] = symbol;
        bytes_read++;
    }

    if (bytes_read != patt_len) {
        free(mas);
        free(str_i);
        free(ind_i);
        return sc_file_error;
    }

    int count = 0;
    if (is_equal_string(pattern, buffer, patt_len)) {
        ind_i[count] = 1;
        str_i[count] = 1;
        count++;
    }
    char_n++;

    while ((symbol = fgetc(input)) != EOF) {
        move_char_left(buffer,patt_len);
        move_int_left(mas, patt_len);

        if (symbol == '\n') {
            string_n++;
            mas[patt_len - 1] = 1;
        }
        else if (patt_len - 2 > 0) {
            mas[patt_len - 1] = mas[patt_len - 2] + 1;
        }
        else {
            mas[patt_len - 1] = mas[patt_len - 1] + 1;
        }
        buffer[patt_len - 1] = symbol;
        if (is_equal_string(pattern, buffer, patt_len)) {
            if (buffer_size < count + 1) {
                buffer_size *= 2;
                int* new_str_i = (int*)realloc(str_i, sizeof(int) * buffer_size);
                if (new_str_i == NULL) {
                    free(str_i);
                    free(ind_i);
                    return sc_memory_error;
                }
                str_i = new_str_i;

                int* new_ind_i = (int*)realloc(ind_i, sizeof(int) * buffer_size);
                if (new_ind_i == NULL) {
                    free(str_i);
                    free(ind_i);
                    return sc_memory_error;
                }
                ind_i = new_ind_i;
            }
            ind_i[count] = mas[0];
            str_i[count] = string_n - fake_newline;
            count++;
        }
        char_n++;
    }

    CELL* cell = (CELL*)malloc(sizeof(CELL));
    cell -> char_pos = ind_i;
    cell -> count = count;
    cell -> string_pos = str_i;
    cell -> state = sc_correct;
    *result = cell;

    free(mas);
    return sc_correct;
}

status_code find_all_patterns(CELL*** result, char* pattern, int count, ...) {
    int patt_len = strlen(pattern);

    char* buffer = (char*)malloc(sizeof(char) * (patt_len + 1));
    if (buffer == NULL) {
        return sc_memory_error;
    }

    va_list args;
    va_start(args, count);
    CELL** cells = (CELL**)malloc(sizeof(CELL*) * count);
    for (int i = 0; i < count; i++) {
        cells[i] = NULL;
    }

    for (int i = 0; i < count; i++) {
        char* path = va_arg(args, char*);
        FILE* input = fopen(path, "r");
        if (input == NULL) {
            free_CELLs(cells, count);
            return sc_file_error;
        }

        CELL* local_result = NULL;
        status_code state = find_pattern(pattern, buffer, patt_len, input, &local_result);
        fclose(input);
        if (state != sc_correct) {
            cells[i] = NULL;
            continue;
        }
        local_result -> path = path;
        cells[i] = local_result;
    }
    va_end(args);
    free(buffer);
    *result = cells;
    return sc_correct;
}
