#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct branch {
    branch_p left_branch;
    char value;
    branch_p right_branch;
} branch_s;

branch_p merge_tree(branch_p left, char value, branch_p right) {
    branch_p branch = malloc(sizeof(branch_s));
    branch->left_branch = left;
    branch->value = value;
    branch->right_branch = right;
    return branch;
}

branch_p create_tree(void) { return NULL; }

char read_branch(branch_p branch) { return branch->value; }

void remove_branch(branch_p branch) {
    if (branch->left_branch == NULL & branch->right_branch == NULL) {
        free(branch);
        return;
    }
    if (branch->left_branch != NULL) {
        remove_branch(branch->left_branch);
    }
    if (branch->right_branch != NULL) {
        remove_branch(branch->right_branch);
    }
}

branch_p left_branch(branch_p branch) { return branch->left_branch; }

branch_p right_branch(branch_p branch) { return branch->right_branch; }

void print_inner(branch_p branch) {
    if (branch == NULL) {
        return;
    }
    if (branch->left_branch == NULL && branch->right_branch == NULL) {
        printf("%c", branch->value);
        return;
    }
    printf("(");
    print_inner(branch->left_branch);
    printf("%c", branch->value);
    print_inner(branch->right_branch);
    printf(")");
}

void print_branch(branch_p branch) {
    print_inner(branch);
    puts("");
}
