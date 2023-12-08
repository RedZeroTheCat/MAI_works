#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Request must include: %s <path>\n", argv[0]);
        return 1;
	}

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        print_error(file_error);
        return 1;
    }

    printf("Enter substring: ");
    char substr_buf[50];
    switch (get_input(substr_buf, sizeof(substr_buf))) {
        case invalid_input:
            print_error(invalid_input);
            return 1;
        default:
            break;
    }
    char* substr = strtok(substr_buf, "\n");
    if (substr == NULL) {
        print_error(invalid_input);
        return 1;
    }

    char buffer[50];
    for (int i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = '\0';
    }
    int found = 0;
    int file_count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        ++file_count;
    }
    rewind(file);

    for (int i = 0; i < file_count; ++i) {
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            break;
        }
        char* input = strtok(buffer, "\n");
        if (input == NULL) {
            print_error(invalid_input);
            return 1;
        }
        int pid = fork();
        if (pid == 0) {
            if (search_substr(1, substr, buffer)) {
                printf("In file: %s\n", buffer);
                return 8976; // found code = 4096
            }
            return 8977; // not found code = 4352
        }
        else if (pid == -1) {
            print_error(fork_error);
            return 1;
        }
    }

    for (int i = 0; i < file_count; ++i) {
        // ждём завершения дочерних процессов
        int status;
        wait(&status);
        if (status != 4096 && status != 4352) {
            printf("Session ended accidently\n");
            printf("Child process stat = %d\n", status);
        }
        else if (status == 4096) {
            found = 1;
        }
    }

    if (!found) {
        printf("No such substring in any file\n");
    }
    fclose(file);
    return 0;
}
