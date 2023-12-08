#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

const int LOGIN_LIMIT = 6;
const int PASSWORD_LIMIT = 6;
const int SANCTIONS_LIMIT = 6;
const int BUFFER_SIZE = 24;

int check_sym(char sym) {
    if ('0' <= sym && sym <= '9') {
        return 1;
    }
    if ('a' <= sym && sym <= 'z') {
        return 1;
    }
    if ('A' <= sym && sym <= 'Z') {
        return 1;
    }
    return 0;
}

int validate_auth_data(const char login[], const char password[]) {
    if (login == NULL || password == NULL) {
        return 0;
    }
    if (login[0] == '\0' || password[0] == '\0') {
        return 0;
    }
    for (int i = 0; i <= LOGIN_LIMIT; ++i) {
        if (login[i] == '\0') {
            break;
        }
        if (i == LOGIN_LIMIT) {
            return 0;
        }
        if (!check_sym(login[i])) {
            return 0;
        }
    }
    for (int i = 0; i <= PASSWORD_LIMIT; ++i) {
        if (password[i] == '\0') {
            break;
        }
        if (i == PASSWORD_LIMIT) {
            return 0;
        }
        if (!isdigit(password[i])) {
            return 0;
        }
    }
    int pass_num = (int)strtol(password, NULL, 10);
    if (!(0 <= pass_num && pass_num <= 100000)) {
        return 0;
    }
    return 1;
}

int auth(const char login[], const char password[], int* sanctions) {
    FILE *sanctions_file;
    char filename[LOGIN_LIMIT + 15];
    for (int i = 0; i < LOGIN_LIMIT + 15; ++i) {
        filename[i] = '\0';
    }
    strcat(filename, login);
    strcat(filename, "-sanctions.txt");
    sanctions_file = fopen(filename, "r");
    if (sanctions_file == NULL) {
        *sanctions = 0;
    }
    else {
        char buffer[BUFFER_SIZE];
        for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
        if (fgets(buffer, sizeof(buffer), sanctions_file) != NULL) {
            *sanctions = atoi(buffer);
        }
        else {
            *sanctions = 0;
        }
        fclose(sanctions_file);
    }

    FILE *file;
    file = fopen("logins.txt", "r");
    if (file == NULL) {
        return 3;
    }

    char buffer[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char* buf_login = strtok(buffer, " ");
        char* buf_pass = strtok(NULL, "\n");

        if (buf_login == NULL || buf_pass == NULL) {
            fclose(file);
            return 3;
        }
        else if (strcmp(login, buf_login) == 0) {
            if (strcmp(password, buf_pass) == 0) {
                fclose(file);
                return 1;
            }
            else {
                fclose(file);
                return 2;
            }
        }
    }
    fclose(file);
    return 0;
}

int reg(const char login[], const char password[]) {
    FILE *file;
    file = fopen("logins.txt", "a");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "%s %s\n", login, password);
    fflush(file);
    fclose(file);
    return 1;
}

int login(int* sanctions) {
    while (1) {
        printf("Enter 'login password'. If you want to leave, enter '!EXIT! '\n: ");
        char buffer[BUFFER_SIZE];
        for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("buffer overflow\n");
            return 0;
        }
        if (!(buffer[BUFFER_SIZE - 2] == '\0' || buffer[BUFFER_SIZE - 2] == '\n')) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        char* login = strtok(buffer, " ");
        char* password = strtok(NULL, "\n");

        if (strcmp(login, "!EXIT!") == 0) {
            printf("Closing program\n");
            return 0;
        }

        if (!validate_auth_data(login, password)) {
            printf("Entered data is incorrect\n");
        }
        else if (auth(login, password, sanctions) == 1) {
            printf("Welcome, %s!\n", login);
            break;
        }
        else if (auth(login, password, sanctions) == 2) {
            printf("Wrong password %s\n", login);
        }
        else if (auth(login, password, sanctions) == 3) {
            printf("Unable to open login file\n");
            return 0;
        }
        else {
            printf("User %s does not exist\n", login);
            printf("Want to register? (y/n)\n");
            char answer[2] = "n";
            if (!fgets(answer, 2, stdin)) {
                printf("- buffer overflow\n");
                return 0;
            }
            if (answer[0] == 'y') {
                if (reg(login, password)) {
                    printf("User %s registred, password: %s\n", login, password);
                } else {
                    printf("Registration canceled\n");
                }
            }
            int tmp;
            while ((tmp = getchar()) != '\n' && tmp != EOF);
        }
    }
    return 1;
}

int set_sanction(const char login[], int sanctions) {
    FILE *file;

    char filename[LOGIN_LIMIT + 15];
    for (int i = 0; i < LOGIN_LIMIT + 15; ++i) filename[i] = '\0';
    strcat(filename, login);
    strcat(filename, "-sanctions.txt");

    file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "%d\n", sanctions);
    fflush(file);
    fclose(file);
    return 1;
}

int start_command_session(int sanctions) {
    sanctions = sanctions ? sanctions : -1;
    while (sanctions) {
        sanctions = sanctions > 0 ? sanctions - 1 : -1;
        printf("> ");

        char buffer[BUFFER_SIZE];
        for (int i = 0; i < BUFFER_SIZE - 1; ++i) buffer[i] = '\0';
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Buffer overflow\n");
            return 0;
        }
        if (!(buffer[BUFFER_SIZE - 2] == '\0' || buffer[BUFFER_SIZE - 2] == '\n')) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        char* command = strtok(buffer, " \n");
        if (command == NULL) {
            ++sanctions;
        }
        else if (strcmp(command, "Logout") == 0) {
            printf("Exit\n\n");
            return 1;
        }
        else if (strcmp(command, "Date") == 0) {
            time_t rawtime = time(NULL);
            struct tm now = *localtime(&rawtime);
            printf("Date right now: %02d:%02d:%4d\n", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
        }
        else if (strcmp(command, "Time") == 0) {
            time_t rawtime = time(NULL);
            struct tm now = *localtime(&rawtime);
            printf("Time right now: %02d:%02d:%02d\n", now.tm_hour, now.tm_min, now.tm_sec);
        }
        else if (strcmp(command, "Howmuch") == 0) {
            char* day_str = strtok(NULL, ":");
            char* month_str = strtok(NULL, ":");
            char* year_str = strtok(NULL, " ");
            char* flag = strtok(NULL, " \n");

            int date_is_valid = 0;
            struct tm pastTm = {0};
            if (day_str != NULL && month_str != NULL && year_str != NULL && flag != NULL) {
                date_is_valid = 1;
                int day = atoi(day_str);
                int month = atoi(month_str);
                int year = atoi(year_str);

                if (!(1 <= day && day <= 31)) {
                    date_is_valid = 0;
                }
                if (!(1 <= month && month <= 12)) {
                    date_is_valid = 0;
                }
                if (year == 0) {
                    date_is_valid = 0;
                }
                if (year < 0) {
                    ++year;
                }
                pastTm.tm_mday = day;
                pastTm.tm_mon = month - 1;
                pastTm.tm_year = year - 1900;
            }
            time_t now = time(NULL);
            time_t past = mktime(&pastTm);

            if (past == -1 || !date_is_valid) {
                printf("Failed to transform date\n");
            }
            else if (past > now) {
                printf("Entered date has not yet come\n");
            }
            else {
                time_t sec_differ = difftime(now, past);

                if (strcmp(flag, "-s") == 0) {
                    printf("Diff in seconds: %ld\n", sec_differ);
                }
                else if (strcmp(flag, "-m") == 0) {
                    printf("Diff in minutes: %ld\n", sec_differ/60);
                }
                else if (strcmp(flag, "-h") == 0) {
                    printf("Diff in hours: %ld\n", sec_differ/60/60);
                }
                else if (strcmp(flag, "-y") == 0) {
                    double yearDiff = sec_differ/365.25/60/60/24;
                    printf("Diff in years: %ld\n", (long)yearDiff);
                }
                else {
                    printf("flag is unexpected\n");
                }
            }
        }
        else if (strcmp(command, "Sanctions") == 0) {
            printf("Confirm your action ('12345'): ");

            char buffer2[BUFFER_SIZE];
            for (int i = 0; i < BUFFER_SIZE - 1; ++i) buffer2[i] = '\0';
            if (!fgets(buffer2, sizeof(buffer2), stdin)) {
                printf("Buffer overflow\n");
                return 0;
            }
            if (!(buffer2[BUFFER_SIZE - 2] == '\0' || buffer2[BUFFER_SIZE - 2] == '\n')) {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            if (strcmp(buffer2, "12345\n") == 0) {
                printf("Accepted\n");
            }
            else {
                printf("Denied\n");
                continue;
            }
            char* login = strtok(NULL, " ");
            char* count_string = strtok(NULL, " \n");

            int sanction_is_valid = 0;
            int count = 0;
            if (login != NULL && count_string != NULL) {
                sanction_is_valid = 1;
                count = atoi(count_string);
                if (!(0 <= count && count <= 100000)) {
                    printf("Количество санкций [0,100000]\n");
                    sanction_is_valid = 0;
                }
            }

            if (!sanction_is_valid) {
                printf("Failed to read command\n");
            }
            else if (!set_sanction(login, count)) {
                printf("Failed to set sanctionsn");
            }
            else {
                printf("Sanctions set\n");
            }
        }
        else {
            printf("Command not found\n");
            ++sanctions;
        }
    }
    printf("Exit\n\n");
    return 1;
}
