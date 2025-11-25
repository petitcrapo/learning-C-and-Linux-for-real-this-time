#ifndef BINARY_TREE_H
#define BINARY_TREE_H


typedef struct branch branch_s;
typedef branch_s* branch_p;
typedef branch_p* tree;


branch_p create_tree(void);
branch_p add_branch(branch_p branch, int side, char value);
char read_branch(branch_p branch);
void remove_branch(branch_p branch);
branch_p left_branch(branch_p branch);
branch_p right_branch(branch_p branch);

#endif 