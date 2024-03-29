#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        print_state(invalid_argument, NULL, stdout, NULL, NULL, NULL, NULL);
        return 0;
    }

    int counter = 0;
    int counter_writting_file = 0;
    for(int i = 1; i < argc; ++i) {
        FILE* file = fopen(argv[i], "r");
        if(file == NULL) {
            print_state(problem_with_file_opening, argv[i], stdout, NULL, NULL, NULL, NULL);
            return 0;
        }
        char* error_filename = generate_new_filename(argv[i]);
        if(error_filename == NULL) {
            fclose(file);
            print_state(memory_error, NULL, stdout, NULL, NULL, NULL, NULL);
            return 0;
        }

        FILE* error_file = fopen(error_filename, "w");
        if(error_file == NULL) {
            fclose(file);
            print_state(problem_with_file_opening, error_filename, stdout, NULL, NULL, NULL, NULL);
            free(error_filename);
            return 0;
        }
        status_code st;
        char* line = read_line(file, &st);
        while(line != NULL && st == correct) {
            if(strlen(line) < 1) {
                free(line);
                line = read_line(file, &st);
                continue;
            }
            counter++;

            if(!check_brackets(line)) {
                int tmp = counter - 1;
                print_state(wrong_brackets, argv[i], error_file, &tmp, line, NULL, NULL);
                counter_writting_file++;
                free(line);
                line = read_line(file, &st);
                continue;
            }
            char* post = NULL;
            st = infix_to_postfix(line, &post);
            if(st == invalid_symbol) {
                int tmp = counter - 1;
                print_state(invalid_symbol, argv[i], error_file, &tmp, line, NULL, NULL);
                counter_writting_file++;
                free(line);
                line = read_line(file, &st);
                continue;
            }
            else if (st != correct) {
                print_state(memory_error, NULL,stdout, NULL, NULL, NULL, NULL);
                free(line);
                fclose(file);
                fclose(error_file);
                if(counter_writting_file == 0) {
                    remove(error_filename);
                }
                free(error_filename);
                return 0;
            }

            long long int result = 0;
            int len = (int)strlen(post);
            char* clone = (char*)malloc(sizeof(char)*(len+1));
            if(clone == NULL) {
                print_state(memory_error, NULL, stdout, NULL, NULL, NULL, NULL);
                free(line);
                free(post);
                fclose(file);
                fclose(error_file);
                if(counter_writting_file == 0) {
                    remove(error_filename);
                }
                free(error_filename);
                return 0;
            }
            strcpy(clone, post);

            st = calculate_postfix(clone, &result);
            free(clone);
            if (st == overflow) {
                print_state(overflow, argv[i],error_file, &counter, line, post, NULL);
                counter_writting_file++;
                free(line);
                free(post);
                line = read_line(file, &st);
                continue;
            }
            if (st == memory_error) {
                print_state(memory_error, NULL, stdout, NULL, NULL, NULL, NULL);
                free(line);
                free(post);
                fclose(file);
                fclose(error_file);
                if(counter_writting_file == 0) {
                    remove(error_filename);
                }
                free(error_filename);
                return 0;
            }

            if (st == invalid_symbol) {
                print_state(invalid_symbol, argv[i], error_file, &counter, line, post, NULL);
                counter_writting_file++;
                free(line);
                free(post);
                line = read_line(file, &st);
                continue;
            }

            if (st == division_by_zero) {
                print_state(division_by_zero, argv[i], error_file, &counter, line, post, NULL);
                counter_writting_file++;
                free(line);
                free(post);
                line = read_line(file, &st);
                continue;
            }

            print_state(correct, argv[i], stdout, NULL, line, post, &result);
            counter_writting_file++;

            free(post);
            free(line);
            line = read_line(file, &st);
        }

        if (st == memory_error) {
            fclose(error_file);
            fclose(file);
            free(error_filename);
            print_state(memory_error, NULL, stdout, NULL, NULL, NULL, NULL);
            return 0;
        }
        if (st == last_str && counter == 0) {
            print_state(empty_file, argv[i], stdout, NULL, NULL, NULL, NULL);
            fclose(file);
            fclose(error_file);
            remove(error_filename);
            free(error_filename);
            continue;
        }
        if (st == last_str) {
            fclose(file);
            counter = 0;
            fclose(error_file);
            if(counter_writting_file == 0) {
                remove(error_filename);
            }
            free(error_filename);
            counter_writting_file = 0;
            continue;
        }
    }
}
