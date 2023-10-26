#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Um...\n");
    printf("Hm ");
    fork();
    printf("Hello? \n");
    fork();
    printf("Is someone there? \n");
}
