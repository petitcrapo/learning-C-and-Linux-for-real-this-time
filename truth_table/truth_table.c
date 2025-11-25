#include "binary_tree.h"
#include <stdio.h>


branch_p create_syntax_tree(char *input) {
    branch_p branch = create_tree();
    
    return branch;
}


char evaluate_tree(branch_p branch, char *var_state) {
    switch (read_branch(branch)) {
    case 'A': {
        return var_state[0];
    }
    case 'B': {
        return var_state[1];
    }
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
    }
    return -1;
}


int main(int argc, char *argv[]) {
    branch_p tree1 = create_tree();
    tree1 = add_branch(tree1, 0, '&');
    add_branch(tree1, 1, 'A');
    add_branch(tree1, 0, 'B');
    branch_p tree2 = create_tree();
    tree2 = add_branch(tree2, 0, '|');
    add_branch(tree2, 1, 'A');
    add_branch(tree2, 0, 'B');
    branch_p tree3 = create_tree();
    tree3 = add_branch(tree3, 0, '&');
    add_branch(tree3, 0, '!');
    add_branch(tree3, 1, '!');
    add_branch(left_branch(tree3), 0, 'A');
    add_branch(right_branch(tree3), 0, 'B');
    char var_state[2];
    printf("_A_B_R_  _A_B_R_  _A_B_R_\n");
    for (int i = 0; i < 4; i++) {
        var_state[0] = (i & 0x02) >> 1;
        var_state[1] = i & 0x01;
        printf("|%d|%d|%d|  |%d|%d|%d|  |%d|%d|%d|\n", var_state[0],
               var_state[1], evaluate_tree(tree1, var_state), var_state[0],
               var_state[1], evaluate_tree(tree2, var_state), var_state[0],
               var_state[1], evaluate_tree(tree3, var_state));
    }
    printf("-------  -------  -------\n");
    return 0;
}
