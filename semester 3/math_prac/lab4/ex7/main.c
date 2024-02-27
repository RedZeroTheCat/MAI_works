#include "functions.c"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Yuor request must include: <%s> <filepath>\n", argv[0]);
        return 1;
    }
    status_code state = run(argv[1]);
    print_error(state);
    return state;
}