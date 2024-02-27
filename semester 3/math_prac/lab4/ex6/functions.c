#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 12
#define MIN_LEN 1
#define TXT 4

typedef enum {
    correct,
    memory_error,
    empty_file,
    problem_with_file_opening,

    wrong_brackets,
    wrong_operation,
    invalid_argument,
    invalid_symbol,
    not_1_string_in_file
} status_code;

typedef struct Stack_node{
    char symbol;
    struct Stack_node* next;
} Stack_node;

typedef struct Stack{
    Stack_node* top;
    Stack_node* bottom;
} Stack;

typedef struct Node{
    char symbol;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Tree{
    Node* root;
} Tree;

typedef struct Node_stack{
    Node* node;
    struct Node_stack* next;
} Node_stack;

typedef struct Tree_stack{
    Node_stack* top;
    Node_stack* bottom;
} Tree_stack;

void print_state(status_code st) {
    switch(st) {
        case correct:
            printf("Correct\n");
            break;
        case memory_error:
            printf("Memory problem\n");
            break;
        case empty_file:
            printf("Empty file or memory allocation problem\n");
            break;
        case problem_with_file_opening:
            printf("Problem with opening file\n");
            break;
        case wrong_brackets:
            printf("Wrong brackets\n");
            break;
        case wrong_operation:
            printf("Wrong operation in postfix\n");
            break;
        case invalid_argument:
            printf("Invalid argument\n");
            break;
        case invalid_symbol:
            printf("Invalid symbol\n");
            break;
        case not_1_string_in_file:
            printf("There is not 1 string in file, check input file!\n");
            break;
        default:
            printf("Unknown state\n");
            break;
    }
}
char* read_line(FILE* file) {
    int size = 0;
    int buf = 10;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL) {
        return NULL;
    }
    int ch;
    while((ch = fgetc(file)) != EOF && ch != '\n' && ch != '\r'){
        size++;
        if(size >= buf) {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL) {
                free(result);
                return NULL;
            }
            else {
                result = tmp;
            }
        }
        result[size-1] = (char)ch;
    }
    if(size == 0 && ch == EOF) {
        free(result);
        return NULL;
    }
    result[size] = '\0';
    return result;
}

bool check_brackets(char* str) {
    char* ptr = str;
    int brackets = 0;
    while(*ptr != '\0') {
        if(*ptr == '(') {
            brackets++;
        }
        else if (*ptr == ')') {
            brackets--;
        }
        ptr++;
    }
    if(brackets != 0) {
        return false;
    }
    return true;
}

status_code push(Stack* stack, char symbol) {
    if(stack->bottom == NULL) {
        stack->bottom = (Stack_node*)malloc(sizeof(Stack_node));
        if(stack->bottom == NULL) {
            return memory_error;
        }
        stack->bottom->symbol = symbol;
        stack->bottom->next = NULL;
        stack->top = stack->bottom;
        return correct;
    }
    Stack_node* node = (Stack_node*)malloc(sizeof(Stack_node));
    if(node == NULL) {
        return memory_error;
    }
    stack->top->next = node;
    stack->top = node;
    stack->top->symbol = symbol;
    stack->top->next = NULL;
    return correct;
}

status_code pop(Stack* stack) {
    if(stack->bottom == NULL) {
        return invalid_argument;
    }
    if(stack->bottom->next == NULL) {
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        return correct;
    }
    Stack_node* tmp = stack->bottom;
    while(tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    stack->top = tmp;
    free(tmp->next);
    tmp->next = NULL;
    return correct;
}

int priority_of_symbol(char symbol) {
    switch(symbol) {
        case'~':
            return 3;
        case '?':
        case '!':
        case '+':
        case '&':
            return 2;
        case '|':
        case '-':
        case '<':
        case '=':
            return 1;
        default:
            return 0;
    }
}

void free_stack(Stack* stack) {
    while(stack->top != NULL) {
        pop(stack);
    }
    free(stack);
}

void print_tree(Node* root, int level) {
    if (root == NULL) {
        return;
    }
    print_tree(root->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%c\n", root->symbol);
    print_tree(root->left, level + 1);
}

bool is_operator(char symbol) {
    return (symbol == '&'
         || symbol == '|'
         || symbol == '?'
         || symbol == '<'
         || symbol == '~'
         || symbol == '+'
         || symbol == '-'
         || symbol == '='
         || symbol == '!');
}

Node* create_node_tree(char symbol) {
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL) {
        return NULL;
    }
    node->symbol = symbol;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree(Node* root) {
    if(root == NULL) {
        return;
    }
    if(root->left != NULL) {
        free_tree(root->left);
    }
    if(root->right != NULL) {
        free_tree(root->right);
    }
    free(root);
}

status_code push_tree_stack(Tree_stack* stack, Node* node) {
    if(stack->bottom == NULL) {
        stack->bottom = (Node_stack *)malloc(sizeof(Node_stack));
        if(stack->bottom == NULL) {
            return memory_error;
        }
        stack->bottom->node = node;
        stack->bottom->next = NULL;
        stack->top = stack->bottom;
        return correct;
    }
    Node_stack* node_stack = (Node_stack*)malloc(sizeof(Node_stack));
    if(node_stack == NULL) {
        return memory_error;
    }
    node_stack->node = node;
    node_stack->next = NULL;
    stack->top->next = node_stack;
    stack->top = node_stack;
    return correct;
}

status_code pop_tree_stack_full(Tree_stack* stack) {
    if (stack->bottom == NULL) {
        return invalid_argument;
    }
    if (stack->bottom->next == NULL) {
        free_tree(stack->bottom->node);
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        return correct;
    }
    Node_stack *tmp = stack->bottom;
    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    stack->top = tmp;
    free_tree(tmp->next->node);
    free(tmp->next);
    tmp->next = NULL;
    return correct;
}

status_code pop_tree_stack(Tree_stack* stack) {
    if (stack->bottom == NULL) {
        return invalid_argument;
    }
    if (stack->bottom->next == NULL) {
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        return correct;
    }
    Node_stack *tmp = stack->bottom;
    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    stack->top = tmp;
    free(tmp->next);
    tmp->next = NULL;
    return correct;
}

void free_tree_stack(Tree_stack* stack) {
    while(stack->top != NULL) {
        pop_tree_stack(stack);
    }
    free(stack);
}

void free_tree_stack_full(Tree_stack* stack) {
    while(stack->top != NULL) {
        pop_tree_stack_full(stack);
    }
    free(stack);
}

char* generate_filename() {
    srand(time(NULL));
    int len = MIN_LEN + rand() % (MAX_LEN - MIN_LEN + 1);
    char *file = (char *)malloc((len + 5) * sizeof(char));
    if(file == NULL) {
        return NULL;
    }
    char symbols[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int count_symbols = sizeof(symbols) - 1;
    for (int i = 0; i <= len; ++i) {
        int index = rand() % count_symbols;
        file[i] = symbols[index];
    }
    file[len] = '\0';
    strcat(file, ".txt");
    return file;
}

status_code infix_to_postfix(char* str, char** postfix) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(stack == NULL) {
        return memory_error;
    }
    stack->bottom = NULL;
    stack->top = NULL;

    int size = 0;
    int buf = 1;
    char* result = (char*)malloc(sizeof(char)*(buf+1));
    if(result == NULL) {
        free_stack(stack);
        return memory_error;
    }
    result[size] = '\0';

    status_code st;
    char* ptr = str;
    bool is_double = false;
    while(*ptr != '\0') {
        if (*ptr == ' ' || *ptr == '\t') {
            ptr++;
            continue;
        }
        if ((isalpha(*ptr) || *ptr == '1' || *ptr == '0') && !is_double) {
            is_double = true;
            if(size >= buf - 1) {
                buf *= 2;
                char* tmp = (char*) realloc(result, sizeof(char) * (buf + 1));
                if (tmp == NULL) {
                    free(result);
                    free_stack(stack);
                    return memory_error;
                }
                result = tmp;
            }
            result[size] = *ptr;
            size++;
        }
        else if (*ptr == '(') {
            is_double = false;
            st = push(stack, *ptr);
            if (st != correct) {
                free(result);
                free_stack(stack);
                return st;
            }
        }
        else if (*ptr == ')') {
            is_double = false;
            while (stack->top->symbol != '(') {
                if (size >= buf - 1) {
                    buf *= 2;
                    char *tmp = (char *) realloc(result, sizeof(char) * (buf + 1));
                    if (tmp == NULL) {
                        free(result);
                        free_stack(stack);
                        return memory_error;
                    }
                    result = tmp;
                }
                result[size] = stack->top->symbol;
                size++;
                st = pop(stack);
                if (st != correct) {
                    free(result);
                    free_stack(stack);
                    return st;
                }
            }
            st = pop(stack);
            if (st != correct) {
                free(result);
                free_stack(stack);
                return st;
            }
        }
        else if(*ptr == '+' || *ptr == '-' || *ptr == '<') {
            is_double = false;
            char *tmp = ptr;
            tmp++;
            if (*tmp != '>') {
                free(result);
                free_stack(stack);
                return invalid_symbol;
            }
            while (stack->top != NULL
            && priority_of_symbol(stack->top->symbol) >= priority_of_symbol(*ptr)) {
                if (size >= buf - 1) {
                    buf *= 2;
                    char *temp = (char *) realloc(result, sizeof(char) * (buf + 1));
                    if (temp == NULL) {
                        free(result);
                        free_stack(stack);
                        return memory_error;
                    }
                    result = temp;
                }
                result[size] = stack->top->symbol;
                size++;
                st = pop(stack);
                if (st != correct) {
                    free(result);
                    free_stack(stack);
                    return st;
                }
            }
            st = push(stack, *ptr);
            if (st != correct) {
                free(result);
                free_stack(stack);
                return st;
            }
            ptr++;
        }
        else if(*ptr == '&'
             || *ptr == '|'
             || *ptr == '~'
             || *ptr == '='
             || *ptr == '!'
             || *ptr == '?') {
            is_double = false;
            while(stack->top != NULL
            && priority_of_symbol(stack->top->symbol) >= priority_of_symbol(*ptr)) {
                if(size >= buf - 1) {
                    buf *= 2;
                    char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
                    if(tmp == NULL) {
                        free(result);
                        free_stack(stack);
                        return memory_error;
                    }
                    result = tmp;
                }
                result[size] = stack->top->symbol;
                size++;
                st = pop(stack);
                if(st != correct) {
                    free(result);
                    free_stack(stack);
                    return st;
                }
            }
            st = push(stack, *ptr);
            if(st != correct) {
                free(result);
                free_stack(stack);
                return st;
            }
        }
        else {
            free(result);
            free_stack(stack);
            return invalid_symbol;
        }
        ptr++;
    }
    while(stack->top != NULL) {
        if(stack->top->symbol == '(' || stack->top->symbol == ')') {
            st = pop(stack);
            if(st != correct) {
                return st;
            }
        }
        if(size >= buf - 1) {
            buf *= 2;
            char* tmp = (char*) realloc(result, sizeof(char)*(buf+1));
            if(tmp == NULL) {
                free(result);
                free_stack(stack);
                return memory_error;
            }
            result = tmp;
        }
        result[size] = stack->top->symbol;
        size++;
        st = pop(stack);
        if(st != correct) {
            free(result);
            free_stack(stack);
            return st;
        }
    }
    result[size] = '\0';
    *postfix = result;
    free_stack(stack);
    return correct;
}

status_code get_postfix(char* filename, char** postix) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        return problem_with_file_opening;
    }
    char* line = read_line(file);
    if(line == NULL) {
        fclose(file);
        return empty_file;
    }
    if(strlen(line) < 1) {
        free(line);
        fclose(file);
        return invalid_argument;
    }
    int ch = fgetc(file);
    if(ch != EOF) {
        free(line);
        fclose(file);
        return not_1_string_in_file;
    }
    if(!check_brackets(line)) {
        free(line);
        fclose(file);
        return wrong_brackets;
    }
    char* postfix = NULL;
    status_code result = infix_to_postfix(line, &postfix);
    if(result != correct) {
        free(line);
        fclose(file);
        return result;
    }
    *postix = postfix;
    free(line);
    fclose(file);
    return correct;
}

int get_value_from_tree(Node* root, char* unique_symbols, int count, int* val) {
    if(root == NULL) {
        return -1;
    }
    int left_val = get_value_from_tree(root->left, unique_symbols, count, val);
    int right_val = get_value_from_tree(root->right, unique_symbols, count, val);
    if(isalpha(root->symbol)) {
        for(int i = 0; i < count; ++i) {
            if(unique_symbols[i] == root->symbol) {
                return val[i];
            }
        }
    }

    if (root->symbol == '0' || root->symbol == '1') {
        return root->symbol - '0';
    }
    if(root->symbol == '&') {
        return left_val & right_val;
    }
    if(root->symbol == '|') {
        return left_val | right_val;
    }
    if(root->symbol == '=') {
        return (int)(left_val == right_val);
    }
    if(root->symbol == '~') {
        return !left_val;
    }
    if(root->symbol == '+') {
        return ~(~left_val | right_val);
    }
    if(root->symbol == '-') {
        return left_val <= right_val;
    }
    if(root->symbol == '<') {
        return (int)(left_val != right_val);
    }
    if(root->symbol == '!') {
        return !(left_val & right_val);
    }
    if(root->symbol == '?') {
        return !(left_val | right_val);
    }
    return -1;
}

status_code print_table(Node* root, int count, char* unique_symbols) {
    char* new_filename = generate_filename();
    if(new_filename == NULL) {
        return memory_error;
    }

    FILE* file = fopen(new_filename, "w");
    if(file == NULL) {
        free(new_filename);
        return memory_error;
    }

    int amount_combo = 1 << count;
    int arr_var[amount_combo];
    for(int i = 0; i < amount_combo; ++i) {
        fprintf(file, ">>>>>>>>\n");
        for(int j = 0; j < count; ++j) {
            int bit = (i >> j) & 1;
            fprintf(file, "| %d ", bit);
            arr_var[j] = bit;
        }
        int final = get_value_from_tree(root, unique_symbols, count, arr_var);
        if(final == -1) {
            return invalid_symbol;
        }
        fprintf(file, "|> %d\n", final);
    }
    fprintf(file, ">>>>>>>>\n");
    fclose(file);
    free(new_filename);
    return correct;
}

bool not_used_symbol(const char* unique_symbols, char symbol, int count) {
    for(int i = 0; i < count; i++) {
        if(unique_symbols[i] == symbol) {
            return false;
        }
    }
    return true;
}

int count_unique_symbols(char* postfix, char** unique_symbols, int* nums) {
    int count = 0;
    int buf = 2;
    *unique_symbols = (char*)malloc(sizeof(char)*(buf + 1));
    if(*unique_symbols == NULL) {
        return -1;
    }
    char* ptr = postfix;
    while (*ptr != '\0') {
        if (isalpha(*ptr)
        && not_used_symbol(*unique_symbols, *ptr, count)) {
            if(count >= buf - 2) {
                buf *= 2;
                char* tmp = (char*)realloc(*unique_symbols, sizeof(char)*(buf + 1));
                if(tmp == NULL) {
                    free(*unique_symbols);
                    return -1;
                }
                *unique_symbols = tmp;
            }
            (*unique_symbols)[count++] = *ptr;
        }
        else if (*ptr == '0' || *ptr == '1') {
            (*nums)++;
        }
        ptr++;
    }
    (*unique_symbols)[count] = '\0';
    return count;
}

status_code build_tree(char* postfix, Tree* tree) {
    Tree_stack* stack = (Tree_stack*)malloc(sizeof(Tree_stack));
    if(stack == NULL) {
        return memory_error;
    }
    stack->top = NULL;
    stack->bottom = NULL;

    status_code st;
    char* ptr = postfix;
    while (*ptr != '\0') {
        if (isalpha(*ptr) || *ptr == '1' || *ptr == '0') {
            Node* node = create_node_tree(*ptr);
            if(node == NULL) {
                free_tree_stack_full(stack);
                return memory_error;
            }
            st = push_tree_stack(stack, node);
            if(st != correct) {
                free(node);
                free_tree_stack_full(stack);
                return st;
            }
        }
        else if(is_operator(*ptr)) {
            if(*ptr == '~') {
                Node* node = create_node_tree(*ptr);
                if(node == NULL) {
                    free_tree_stack_full(stack);
                    return memory_error;
                }
                if(stack->top == NULL) {
                    free(node);
                    free_tree_stack_full(stack);
                    return wrong_operation;
                }
                Node* left = stack->top->node;
                st = pop_tree_stack(stack);
                if(st != correct) {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
                node->left = left;
                st = push_tree_stack(stack, node);
                if(st != correct) {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
            }
            else {
                Node* node = create_node_tree(*ptr);
                if(node == NULL) {
                    free_tree_stack_full(stack);
                    return memory_error;
                }
                if(stack->top == NULL ) {
                    free(node);
                    free_tree_stack_full(stack);
                    return wrong_operation;
                }
                Node* right = stack->top->node;
                st = pop_tree_stack(stack);
                if(st != correct) {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
                if(stack->top == NULL ) {
                    free(node);
                    free_tree_stack_full(stack);
                    return wrong_operation;
                }
                Node* left = stack->top->node;
                st = pop_tree_stack(stack);
                if(st != correct) {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
                node->left = left;
                node->right = right;
                st = push_tree_stack(stack, node);
                if(st != correct) {
                    free(node);
                    free_tree_stack_full(stack);
                    return st;
                }
            }
        }
        else {
            free_tree_stack_full(stack);
            return invalid_symbol;
        }
        ptr++;
    }
    if(stack->top != stack->bottom) {
        free_tree_stack_full(stack);
        return wrong_operation;
    }
    tree->root = stack->top->node;
    free_tree_stack(stack);
    return correct;
}
