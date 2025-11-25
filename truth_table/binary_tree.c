#include "binary_tree.h"
#include <stdlib.h>
#include <unistd.h>


typedef struct branch {
  branch_p left_branch;
  char value;
  branch_p right_branch;
} branch_s;

branch_p create_tree(void) {
    return NULL;
}

//goon up
branch_p add_branch(branch_p branch , int side, char value) {
    branch_p next_branch;
    if (branch == NULL) {
        branch = malloc(sizeof(branch_s));
        branch->left_branch = NULL;
        branch->right_branch = NULL;
        next_branch = branch;
    } else {
        if (side) {
            next_branch = malloc(sizeof(branch_s));
            branch->right_branch = next_branch;
        } else {
            next_branch = malloc(sizeof(branch_s));
            branch->left_branch = next_branch;
        }
        next_branch->right_branch = NULL;
        next_branch->left_branch = NULL;
        branch = NULL;
    }
    next_branch->value = value;
    return branch;
}

char read_branch(branch_p branch) {
    return branch->value;
}

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

branch_p left_branch(branch_p branch) {
    return branch->left_branch;
}

branch_p right_branch(branch_p branch) {
    return branch->right_branch;
}
