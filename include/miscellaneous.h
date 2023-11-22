#include <stdio.h>
#include "bin_tree.h"

void treeprint(struct bst_tok_node *root, int level);

void print2DUtil(struct bst_tok_node* root, int space);

void print2D(struct bst_tok_node* root);