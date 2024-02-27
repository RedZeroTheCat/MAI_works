#include <stdio.h>

#include "functions.c"

int main(int argc, char** argv) {
    if (argc != 2) {
        print_state(invalid_argument);
    }
    char* postfix = NULL;
    status_code st = get_postfix(argv[1], &postfix);
    if (st != correct) {
        print_state(st);
        return 0;
    }

    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        free(postfix);
        print_state(memory_error);
        return 0;
    }
    tree->root = NULL;
    st = build_tree(postfix, tree);
    if(st != correct) {
        free(postfix);
        free(tree);
        print_state(st);
        return 0;
    }
    char* unique_symbols = NULL;
    int nums = 0;
    int count_uniq = count_unique_symbols(postfix, &unique_symbols, &nums);
    if(count_uniq < 1) {
        if (nums < 1) {
            free(postfix);
            free_tree(tree->root);
            free(tree);
            printf("Huh\n");
            print_state(invalid_symbol);
            return 0;
        }
    }
    st = print_table(tree->root, count_uniq, unique_symbols);
    if (st != correct) {
        free(postfix);
        free_tree(tree->root);
        free(tree);
        free(unique_symbols);
        print_state(st);
        return 0;
    }
    print_state(correct);
    free(unique_symbols);
    free_tree(tree->root);
    free(tree);
    free(postfix);
}