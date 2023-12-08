#include <stdio.h>

#include "functions.c"

int main() {
    while (1) {
        int sanctions = 0;
        if (!login(&sanctions)) {
            return 1;
        }
        int pid = fork();
        if (pid == 0) {
            if (!start_command_session(sanctions)) {
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
            // ждём завершение дочернего процесса
            int status;
            waitpid(pid, &status, 0);
            if (status != 0) {
                printf("Session ended accidently\n");
            }
        }
    }
    return 0;
}
