#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t current_process_id = getpid();
    pid_t parent_of_current_process_id = getppid();

    gid_t current_group_id = getpgrp();

    uid_t current_user_id = getuid();
    gid_t current_users_group_id = getgid();

    uid_t current_user_effective_id = geteuid();
    gid_t current_users_group_effective_id = getegid();

////////////////////////////////////////////////////////////////////

    printf("Id of current process: %d\n\n", current_process_id);
    printf("Id of parent of current process: %d\n\n", parent_of_current_process_id);

    printf("Id of current group: %d\n\n", current_group_id);

    printf("Id of current user: %d\n\n", current_user_id);
    printf("Id of current users group: %d\n\n", current_users_group_id);

    printf("Effective id of current user: %d\n\n", current_user_effective_id);
    printf("Effective id of current users group: %d\n\n", current_users_group_effective_id);

    return 0;
}
