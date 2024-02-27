#include <stdio.h>

#include "hash.c"

void print_state(status_code st) {
    switch(st){
        case correct:
            printf("Correct\n");
            break;
        case memory_error:
            printf("Memory error\n");
            break;
        case empty_file:
            printf("Empty file\n");
            break;
        case same_file:
            printf("Same file\n");
            break;
        case wrong_amount_of_arguments:
            printf("Wrong amount of arguments\n");
            break;
        case problem_with_file_opening:
            printf("Problem with file opening\n");
            break;
        case last_str:
            printf("Last str\n");
            break;
        case invalid_data:
            printf("Invalid data\n");
            break;
        default:
            printf("Unknown status\n");
            break;
    }
}

bool is_correct_define(char* str) {
    int len = (int)strlen(str);
    for(int i = 0; i < len; i++) {
        if(!isdigit(str[i]) && !isalpha(str[i])) {
            return false;
        }
    }
    return true;
}

char* read_define_line(status_code* st, char* ptr) {
    int size = 0;
    int buf = 20;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL) {
        *st = memory_error;
        return NULL;
    }
    while(*ptr != '\n' && *ptr != '\0') {
        size++;
        if(size >= buf) {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL) {
                free(result);
                *st = memory_error;
                return NULL;
            }
            result = tmp;

        }
        result[size-1] = *ptr;
        ptr++;
    }
    result[size] = '\0';
    *st = correct;
    return result;
}

char* read_line(status_code* stat, FILE* file) {
    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL){
        *stat = memory_error;
        return NULL;
    }
    int ch;
    while((ch = fgetc(file)) != EOF && ch != '\n'){
        size++;
        if(size >= buf) {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL) {
                free(result);
                *stat = memory_error;
                return NULL;
            }
            else {
                result = tmp;
            }
        }
        result[size-1] = (char)ch;
    }
    if(size == 0 && ch == EOF){
        *stat = last_str;
        free(result);
        return NULL;
    }
    result[size] = '\0';

    *stat = correct;
    return result;
}

int count_tokens(char *str, const char *delim) {
    int count = 0;
    char *token = strtok(str, delim);
    while (token != NULL) {
        count++;
        token = strtok(NULL, delim);
    }
    return count;
}

bool in_alphabet(const char symb) {
    return (isalpha(symb) || isdigit(symb));
}

status_code change_string(char **string, const char *old, const char *new) {
    char *str = *string;
    int let = (int)strlen(str);
    int ln = (int)strlen(old);
    int delta = -ln + (int)strlen(new);
    for (int i = 0; i < (let - ln); i++) {
        if ((strncmp(str + i, old, ln) == 0)
        && (i == 0 || !in_alphabet(*(str + i - 1)))
        && (i > let - 1 || !in_alphabet(*(str + i + 1)))) {
            let += delta;
            if (delta > 0) {
                char *temp = (char *)realloc(str, sizeof(char) * (let + 1));
                if (temp == NULL){
                    return memory_error;
                }
                str = temp;
            }
            memmove(str + i + strlen(new), str + i + ln, strlen(str + i + ln) + 1);
            memcpy(str + i, new, strlen(new));
            i += ln - 1;
        }
    }
    *string = str;
    return correct;
}

status_code file_work(char* input_name, char* out_name, HashTable** ht) {
    FILE* input = fopen(input_name, "r");
    if(input == NULL) {
        return problem_with_file_opening;
    }
    int q = fgetc(input);
    if (q == EOF) {
        fclose(input);
        return empty_file;
    }
    else {
        fseek(input, 0, SEEK_SET);
    }

    FILE* out = fopen(out_name, "w");
    if(out == NULL) {
        fclose(input);
        return problem_with_file_opening;
    }

    char* line = NULL;
    status_code st = correct;
    char* ptr = NULL;
    while ((line = read_line(&st, input)) != NULL) {
        ptr = strstr(line, "#define");
        if(ptr != NULL) {
            char* define_line = read_define_line(&st, ptr);
            if(st != correct) {
                fclose(input);
                fclose(out);
                free(line);
                return st;
            }
            char* tmp = (char*)malloc(sizeof(char)*(strlen(line)+1));
            if(tmp == NULL) {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                return memory_error;
            }
            strcpy(tmp, define_line);
            int count = count_tokens(tmp, " ");
            if(count != 3) {
                fclose(input);
                fclose(out);
                free(line);
                free(tmp);
                free(define_line);
                return invalid_data;
            }
            strcpy(tmp, line);
            free(tmp);
            tmp = NULL;

            char* def_name = strtok(define_line, " ");
            def_name = strtok(NULL, " ");
            char* value = strtok(NULL, " ");

            if(!is_correct_define(def_name)) {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                return invalid_data;
            }

            st = balance_insert(ht, def_name, value);
            if(st != correct) {
                fclose(input);
                fclose(out);
                free(line);
                free(define_line);
                return st;
            }
            free(define_line);
        }
        else {
            for(int i = 0; i < (*ht)->size; i++){
                Node* tmp = (*ht)->table[i];
                while(tmp != NULL) {
                    char* temp_str = (char*)malloc(sizeof(char)*(strlen(line)+1));
                    if(temp_str == NULL) {
                        fclose(input);
                        fclose(out);
                        free(line);
                        return memory_error;
                    }
                    strcpy(temp_str, line);
                    st = change_string(&line, tmp->key, tmp->value);
                    if(st != correct){
                        fclose(input);
                        fclose(out);
                        free(line);
                        free(temp_str);
                        return st;
                    }
                    if(strcmp(line, temp_str) != 0){
                        free(temp_str);
                        i = 0;
                        break;
                    }
                    free(temp_str);
                    tmp = tmp->next;
                }
            }
        }
        fprintf(out, "%s\n", line);
        if(line != NULL)
        {
            free(line);
        }
    }
    if(st == memory_error) {
        fclose(input);
        fclose(out);
        free(line);
        return st;
    }
    fclose(input);
    fclose(out);
    return correct;
}
