#include <stdio.h>

#include "functions.c"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        print_status(invalid_count_argc);
        return 0;
    }
    srand((unsigned int)time(NULL));
    status_code st = parse_file(argv[1]);
    if(st != correct) {
        print_status(st);
        return 0;
    }
    print_status(correct);
    return 0;
}
