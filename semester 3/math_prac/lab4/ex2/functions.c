#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <errno.h>


typedef enum status_code {
    correct,
    problem_with_file_opening,
    file_wrote,
    file_read,
    same_file,
    empty_file,

    memory_error,
    invalid_command,
    invalid_index,
    invalid_range,

    empty_array,
    not_existed_array,
    exist_array,

    int_overflow,
    invalid_format,
    invalid_count_argc
} status_code;

typedef struct Massive {
    char name;
    int* data;
    int size;
    int buf;
} Massive;

void print_status(status_code state) {
    switch (state) {
        case correct:
            printf("that's well\n");
            break;
        case problem_with_file_opening:
            printf("Problem to open file\n");
            break;
        case file_wrote:
            printf("Unable to write to the file\n");
            break;
        case file_read:
            printf("Unable to read the file.\n");
            break;
        case same_file:
            printf("Files are the same.\n");
            break;
        case memory_error:
            printf("Memory allocation problem\n");
            break;
        case invalid_command:
            printf("Invalid command.\n");
            break;
        case invalid_index:
            printf("Invalid index.\n");
            break;
        case invalid_range:
            printf("Invalid range.\n");
            break;
        case empty_array:
            printf("Array is empty.\n");
            break;
        case not_existed_array:
            printf("Array does not exist.\n");
            break;
        case exist_array:
            printf("Array already exists.\n");
            break;
        case int_overflow:
            printf("Int overflow.\n");
            break;
        case invalid_format:
            printf("Invalid format.\n");
            break;
        case invalid_count_argc:
            printf("Invalid count of argc.\n");
            break;
        case empty_file:
            printf("File is empty.\n");
            break;
        default:
            printf("Unknown status.\n");
            break;
    }
}

status_code string_to_int(char* str, int* value) {
    char* end;
    errno = 0;
    long int conv = strtol(str, &end, 10);
    if(end == str || (*end != '\r' && *end != '\n' && *end != '\0')) {
        return invalid_format;
    }
    if ((errno == ERANGE && (conv == LONG_MAX || conv == LONG_MIN))
    || (errno != 0 && conv == 0)) {
        return int_overflow;
    }
    if(conv > INT_MAX || conv < INT_MIN) {
        return int_overflow;
    }
    *value = (int)conv;
    return correct;
}

status_code free_array(Massive** array) {
    if(*array == NULL) {
        return empty_array;
    }
    if((*array)->data != NULL) {
        free((*array)->data);
    }
    (*array)->data = NULL;
    (*array)->size = 0;
    (*array)->buf = 0;
    free(*array);
    *array = NULL;
    return correct;
}

status_code create_new_array(Massive** arr, char name, int buf) {
    if(buf <= 0) {
        return invalid_command;
    }
    *arr = (Massive*)malloc(sizeof(Massive));
    if(*arr == NULL) {
        return memory_error;
    }
    (*arr)->data = (int*)malloc(sizeof(int) * buf);
    if((*arr)->data == NULL) {
        free(*arr);
        *arr = NULL;
        return memory_error;
    }
    (*arr)->size = 0;
    (*arr)->buf = buf;
    (*arr)->name = name;
    return correct;
}

void free_all_arrays(Massive* array_storage[], int size) {
    for (size_t i = 0; i < size; i++) {
        if (array_storage[i] != NULL) {
            free_array(&array_storage[i]);
        }
    }

}

status_code get_arr_from_file(char* filename, Massive* arr) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return problem_with_file_opening;
    }

    char* line = NULL;
    size_t len = 0;
    while(getline(&line, &len, file) != -1) {
        char* token = strtok(line, " \n\t\r");
        while(token != NULL) {
            int value;
            status_code st = string_to_int(token, &value);
            if(st != correct) {
                fclose(file);
                free(line);
                return st;
            }

            if(arr->size == arr->buf) {
                arr->buf *= 2;
                int* tmp = (int*)realloc(arr->data, sizeof(int) * arr->buf );
                if(tmp == NULL) {
                    fclose(file);
                    free(line);
                    return memory_error;
                }
                arr->data = tmp;
            }
            arr->data[arr->size++] = value;
            token = strtok(NULL, " \n\t\r");
        }
    }
    fclose(file);
    free(line);
    if(arr->size == 0) {
        return empty_file;
    }
    return correct;
}


status_code save_to_file(char* filename, Massive* arr)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return problem_with_file_opening;
    }
    for(int i = 0; i < arr->size; i++) {
        fprintf(file, "%d ", arr->data[i]);
    }
    fclose(file);
    return correct;
}

status_code fill_randomly(Massive* arr, int lb, int rb, int count) {
    if(lb > rb) {
        return invalid_range;
    }
    if(count > arr->buf) {
        arr->buf = count;
        int* tmp = (int*)realloc(arr->data, sizeof(int) * arr->buf);
        if(tmp == NULL) {
            return memory_error;
        }
        arr->data = tmp;
    }
    for(int i = 0; i < count; i++) {
        arr->data[i] = rand() % (rb - lb + 1) + lb;
    }
    arr->size = count;
    return correct;
}

status_code concatination_arrays(Massive* arr_first, Massive* arr_second) {
    int new_size = arr_first->size + arr_second->size;
    if(new_size > arr_first->buf) {
        int* tmp = (int*)realloc(arr_first->data, sizeof(int) * new_size);
        if(tmp == NULL) {
            return memory_error;
        }
        arr_first->data = tmp;
    }
    for(int i = 0; i < arr_second->size; i++) {
        arr_first->data[arr_first->size + i] = arr_second->data[i];
    }
    arr_first->size = new_size;
    return correct;
}

status_code remove_some_elem_from_array(Massive* arr, int start, int count) {
    if(start < 0 || start >= arr->size) {
        return invalid_range;
    }
    if(count < 0 || start + count > arr->size) {
        return invalid_range;
    }
    int new_size = arr->size - count;
    if(new_size < 0) {
        return invalid_range;
    }
    if(new_size == 0) {
        free(arr->data);
        arr->data = NULL;
        arr->size = 0;
        arr->buf = 0;
        return correct;
    }
    int* tmp = (int*)malloc(sizeof(int) * new_size);
    if(tmp == NULL) {
        return memory_error;
    }
    for(int i = 0; i < arr->size; ++i) {
        if(i >= start && i < start + count) {
            continue;
        }
        else if(i < start) {
            tmp[i] = arr->data[i];
        }
        else {
            tmp[i - count] = arr->data[i];
        }
    }
    free(arr->data);
    arr->data = tmp;
    arr->size = new_size;
    return correct;
}

status_code copy_part_arr(Massive* first, Massive* second, int start, int end) {
    if(start < 0 || start >= first->size) {
        return invalid_range;
    }
    if(end < 0 || end >= first->size) {
        return invalid_range;
    }
    if(end < start) {
        return invalid_range;
    }
    free(second->data);
    second->data = NULL;
    second->data = (int*)malloc(sizeof(int) * (end - start + 1));
    if(second->data == NULL) {
        return memory_error;
    }
    second->size = 0;
    second->buf = end - start + 1;
    for(int i = 0; i < end - start + 1; i++) {
        second->data[i] = first->data[start + i];
        second->size++;
    }
    return correct;
}

int compare_plus(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int compare_minus(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}

status_code sort_array(Massive* arr, char type) {
    if(arr->size == 0) {
        return empty_array;
    }
    int (*comp)(const void*, const void*);
    if(type == '-') {
        comp = compare_minus;
    }
    else if(type == '+') {
        comp = compare_plus;
    }
    else {
        return invalid_command;
    }
    qsort(arr->data, arr->size, sizeof(int), comp);
    return correct;
}

status_code shuffle(Massive* arr) {
    if(arr->size == 0) {
        return empty_array;
    }
    for(int i = arr->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr->data[i];
        arr->data[i] = arr->data[j];
        arr->data[j] = tmp;
    }
    return correct;
}

status_code get_max_array(Massive* arr, int* max, int* max_index) {
    if(arr->size == 0) {
        return empty_array;
    }
    *max = arr->data[0];
    *max_index = 0;
    for(int i = 1; i < arr->size; i++) {
        if(arr->data[i] > *max) {
            *max = arr->data[i];
            *max_index = i;
        }
    }
    return correct;
}

status_code get_min_array(Massive* arr, int* min, int* min_index) {
    if(arr->size == 0) {
        return empty_array;
    }
    *min = arr->data[0];
    *min_index = 0;
    for(int i = 1; i < arr->size; i++) {
        if(arr->data[i] < *min) {
            *min = arr->data[i];
            *min_index = i;
        }
    }
    return correct;
}

status_code get_most_often(Massive* arr, int* most_often, int* count) {
    if(arr->size == 0) {
        return empty_array;
    }
    *most_often = arr->data[0];
    *count = 1;
    for(int i = 1; i < arr->size; i++) {
        int current_val = arr->data[i];
        int current_count = 1;
        for(int j = i + 1; j < arr->size; j++) {
            if(arr->data[j] == current_val) {
                current_count++;
            }
        }
        if(current_count > *count
        || (current_count == *count && current_val > *most_often)) {
            *most_often = current_val;
            *count = current_count;
        }
    }
    return correct;
}

status_code get_average_from_array(Massive* arr, double* average) {
    if(arr->size == 0) {
        return empty_array;
    }
    *average = 0;
    for(int i = 0; i < arr->size; i++) {
        *average += arr->data[i];
    }
    *average /= (double )arr->size;
    return correct;
}

status_code get_max_otklonenie(Massive* arr, double* max_otklonenie, int* position, int* number) {
    if(arr->size == 0) {
        return empty_array;
    }
    double average;
    status_code st = get_average_from_array(arr, &average);
    if(st != correct) {
        return st;
    }

    *max_otklonenie = 0;
    for(int i = 1; i < arr->size; i++) {
        double tmp = fabs(arr->data[i] - average);
        if(tmp > *max_otklonenie) {
            *max_otklonenie = tmp;
            *position = i;
            *number = arr->data[i];
        }
    }
    return correct;
}

status_code print_all(Massive* arr) {
    if(arr->size == 0) {
        return empty_array;
    }
    printf("Elements in array %c are: \n", arr->name);
    for(int i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    return correct;
}

void print_stats_array(Massive* arr) {
    int max, min, position, value, count;
    printf("Stats of array: %c\n", arr->name);
    printf("size of array: %d\n", arr->size);

    status_code st = get_max_array(arr, &max, &position);
    if(st != correct) {
        print_status(st);
        return;
    }
    printf("max: %d, position: %d\n", max, position);
    st = get_min_array(arr, &min, &position);
    if(st != correct) {
        print_status(st);
        return;
    }
    printf("min: %d, position: %d\n", min, position);
    st = get_most_often(arr, &value, &count);
    if(st != correct) {
        print_status(st);
        return;
    }
    printf("most often: %d, count: %d\n", value, count);

    double average;
    st = get_average_from_array(arr, &average);
    if(st != correct) {
        print_status(st);
        return;
    }
    printf("average: %f\n", average);

    double max_otklonenie;
    st = get_max_otklonenie(arr, &max_otklonenie, &position, &value);
    if(st != correct) {
        print_status(st);
        return;
    }
    printf("max otklonenie: %f, position: %d, value: %d\n\n", max_otklonenie, position, value);
    print_all(arr);
}

status_code print_one_elem(Massive* arr, int index) {
    if(index < 0 || index >= arr->size) {
        return invalid_index;
    }
    printf("ELem on position %d in array %c is %d\n", index, arr->name, arr->data[index]);
    return correct;
}

status_code print_range(Massive* arr, int start, int end) {
    if(start < 0 || start >= arr->size) {
        return invalid_index;
    }
    if(end < 0 || end >= arr->size) {
        return invalid_index;
    }
    if(start > end) {
        return invalid_range;
    }
    printf("Elements from %d to %d in array %c are: \n", start, end, arr->name);
    for(int i = start; i <= end; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    return correct;
}

status_code parse_file(char* filename) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        return problem_with_file_opening;
    }

    Massive* gen_array[26];
    for(int i = 0; i < 26; i++) {
        gen_array[i] = NULL;
    }
    int size = 0;
    int buf =  1;
    char* line = (char*)malloc(sizeof(char) * (buf+1));
    if(line == NULL) {
        fclose(file);
        return memory_error;
    }
    status_code st;
    int ch;
    while((ch = getc(file)) != EOF) {
        if(line == NULL || size >= buf - 1) {
            buf *= 2;
            char* tmp = realloc(line, sizeof(char) * (buf+1));
            if(tmp == NULL) {
                free(line);
                fclose(file);
                return memory_error;
            }
            line = tmp;
        }
        line[size++] = (char)ch;
        if(ch == ';') {
            line[size] = '\0';
            size = 0;
            char* comand = strtok(line, " \n\t\r(");
            if(comand == NULL) {
                fclose(file);
                free(line);
                free_all_arrays(gen_array, 26);
                return invalid_format;
            }
            if (strcmp(comand, "Load") == 0) {
                char* array_name = strtok(NULL, " ,\n\t\r");
                if(array_name == NULL || strlen(array_name) != 1 || !isalpha(*array_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_format;
                }
                char* filename_new = strtok(NULL, " ;\n\t\r");
                if(filename == NULL || strcmp(filename, filename_new) == 0) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return same_file;
                }
                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                Massive* arr = NULL;
                int index = toupper(*array_name) - 'A';
                if(gen_array[index] != NULL) {
                    free_array(&(gen_array[index]));
                }

                if((st = create_new_array(&arr, *array_name, 1)) != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
                gen_array[index] = arr;

                if((st = get_arr_from_file(filename_new, arr)) != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Save") == 0) {
                char* arr_name = strtok(NULL, " \n\t\r,");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                char* filename_new = strtok(NULL, " ;\n\t\r");
                if(filename == NULL || strcmp(filename, filename_new) == 0) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                Massive* arr = NULL;
                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }

                st = save_to_file(filename_new, gen_array[index]);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Rand") == 0) {
                char* arr_name = strtok(NULL, " \n\t\r,");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                int count;
                int lb;
                int rb;
                char* count_string = strtok(NULL, " ,\n\t\r");
                if(count_string == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                st = string_to_int(count_string, &count);
                if(st != correct || count <= 0) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    if(st == correct) {
                        return invalid_command;
                    }
                    return st;
                }

                char* lb_string = strtok(NULL, " ,\n\t\r");
                if(lb_string == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                st = string_to_int(lb_string, &lb);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
                char* rb_string = strtok(NULL, " ;\n\t\r");
                if(rb_string == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                st = string_to_int(rb_string, &rb);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    st = create_new_array(&(gen_array[index]), *arr_name, 1);
                    if(st != correct) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                }
                else {
                    free_array(&(gen_array[index]));
                    st = create_new_array(&(gen_array[index]), *arr_name, 1);
                    if(st != correct) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                }
                st = fill_randomly(gen_array[index], lb, rb, count);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Concat") == 0) {
                char* arr_name_first = strtok(NULL, " ,\n\t\r");
                if(arr_name_first == NULL || strlen(arr_name_first) != 1 || !isalpha(*arr_name_first)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                char* arr_name_second = strtok(NULL, " ;\n\t\r");
                if(arr_name_second == NULL || strlen(arr_name_second) != 1 || !isalpha(*arr_name_second)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                int index_first = toupper(*arr_name_first) - 'A';
                if(gen_array[index_first] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int index_second = toupper(*arr_name_second) - 'A';
                if(gen_array[index_second] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                st = concatination_arrays(gen_array[index_first], gen_array[index_second]);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Free") == 0) {
                char* arr_name = strtok(NULL, ")");//возможно нужно вернуть (
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                free_array(&gen_array[index]);
            }
            else if(strcmp(comand, "Remove") == 0) {
                char* arr_name = strtok(NULL, " ,\n\t\r");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                char* start = strtok(NULL, " ,\n\t\r");
                char* count = strtok(NULL, " ,;\n\t\r");
                if(start == NULL || count == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int start_int;
                int count_int;
                st = string_to_int(start, &start_int);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                st = string_to_int(count, &count_int);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                st = remove_some_elem_from_array(gen_array[index], start_int, count_int);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Copy") == 0) {
                char* first_arr_name = strtok(NULL, " ,\n\t\r");
                if(first_arr_name == NULL
                || strlen(first_arr_name) != 1
                || !isalpha(*first_arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                char* start = strtok(NULL, " ,\n\t\r");
                char* end = strtok(NULL, " ,\n\t\r");
                if(start == NULL || end == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                char* second_arr_name = strtok(NULL, " ;\n\t\r");
                if(second_arr_name == NULL
                || strlen(second_arr_name) != 1
                || !isalpha(*second_arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                int index_first = toupper(*first_arr_name) - 'A';
                if(gen_array[index_first] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int index_second = toupper(*second_arr_name) - 'A';
                if(gen_array[index_second] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                int start_int;
                int count_int;
                st = string_to_int(start, &start_int);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                st = string_to_int(end, &count_int);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                st = copy_part_arr(gen_array[index_first], gen_array[index_second], start_int, count_int);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Sort") == 0) {
                char* arr_name = strtok(NULL, " ;\n\t\r");
                if (arr_name == NULL
                || strlen(arr_name) != 2
                || !isalpha(arr_name[0])
                || (arr_name[1] != '+'
                && arr_name[1] != '-')) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }

                st = sort_array(gen_array[index], arr_name[1]);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Shuffle") == 0) {
                char* arr_name = strtok(NULL, " \n\t\r;");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                st = shuffle(gen_array[index]);
                if(st != correct) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return st;
                }
            }
            else if(strcmp(comand, "Stats") == 0) {
                char* arr_name = strtok(NULL, " ;\n\t\r");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                if(strtok(NULL, " ;\n\t\r") != NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                print_stats_array(gen_array[index]);
            }
            else if(strcmp(comand, "Print") == 0) {
                char* arr_name = strtok(NULL, " ,\n\t\r");
                if(arr_name == NULL || strlen(arr_name) != 1 || !isalpha(*arr_name)) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }

                int index = toupper(*arr_name) - 'A';
                if(gen_array[index] == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return not_existed_array;
                }
                char* next = strtok(NULL, " ,;\n\t\r");
                if(next == NULL) {
                    fclose(file);
                    free(line);
                    free_all_arrays(gen_array, 26);
                    return invalid_command;
                }
                char* next_next = strtok(NULL, " ;\n\t\r");
                if (strcmp(next, "all") == 0 && next_next == NULL) {
                    st = print_all(gen_array[index]);
                    if(st != correct && st != empty_array) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    if(st == empty_array) {
                        print_status(empty_array);
                    }
                }
                else if(next_next != NULL) {
                    int start, end;
                    st = string_to_int(next, &start);
                    if(st != correct) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    st = string_to_int(next_next, &end);
                    if(st != correct) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    st = print_range(gen_array[index], start, end);
                    if(st != correct) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                }
                else {
                    int current_position;
                    st = string_to_int(next, &current_position);
                    if(st != correct) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                    st = print_one_elem(gen_array[index], current_position);
                    if(st != correct) {
                        fclose(file);
                        free(line);
                        free_all_arrays(gen_array, 26);
                        return st;
                    }
                }
            }
            else {
                fclose(file);
                free(line);
                free_all_arrays(gen_array, 26);
                return invalid_command;
            }
        }
    }
    fclose(file);
    free(line);
    free_all_arrays(gen_array, 26);
    return correct;
}
