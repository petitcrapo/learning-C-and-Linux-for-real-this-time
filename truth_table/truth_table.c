#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

branch_p create_syntax_tree(char **input) {
    branch_p branch = create_tree();
    switch (**input) {
    case '!': {
        ++*input;
        return merge_tree(create_syntax_tree(input), '!', create_tree());
    }
    case '(': {
        ++*input;
        branch_p left = create_syntax_tree(input);
        ++*input;
        char operator = **input;
        ++*input;
        branch_p right = create_syntax_tree(input);
        ++*input;
        return merge_tree(left, operator, right);
    }
    default: {
        if ('A' <= **input && **input <= 'Z') {
            return merge_tree(create_tree(), **input, create_tree());
        } else {
            printf("kys\n");
            abort();
        }
    }
    }
    return branch;
}

char evaluate_tree(branch_p branch, char *var_state) {
    switch (read_branch(branch)) {
    case '!': {
        return !(evaluate_tree(left_branch(branch), var_state));
    }
    case '&': {
        return evaluate_tree(left_branch(branch), var_state) &
               evaluate_tree(right_branch(branch), var_state);
    }
    case '|': {
        return evaluate_tree(left_branch(branch), var_state) |
               evaluate_tree(right_branch(branch), var_state);
    }
    default: {
        if ('A' <= read_branch(branch) && read_branch(branch) <= 'Z') {
            return var_state[read_branch(branch) - 'A']; // ça c'est juste
        }
    }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    printf("\x1b[2J\x1b[H");
    char *a = argv[1];
    branch_p feur = create_syntax_tree(&a);
    print_branch(feur);
    char var_num = 2;
    char var_state[26];
    printf("┏");
    for (int i = 0; i < var_num; i++) {
        printf("━━━┳");
    }
    printf("━━━━┓\n");
    for (int i = 0; i < (1 << var_num); i++) {
        for (int j = 0; j < var_num; j++) {
            var_state[var_num - j - 1] = (i & (1 << j)) >> j;
        }
        for (int k = 0; k < var_num; k++) {
            printf("┃ \x1b[3%dm%c\x1b[39m ", var_state[k] ? 2 : 1,
                   'A' + k);
        }
        printf("┃ %s ┃\n", evaluate_tree(feur, var_state)
                               ? "\x1b[32m██\x1b[39m"
                               : "\x1b[31m██\x1b[39m");
    }
    printf("┗");
    for (int i = 0; i < var_num; i++) {
        printf("━━━┻");
    }
    printf("━━━━┛\n");
    return 0;
}
