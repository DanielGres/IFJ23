#include "miscellaneous.h"

#define COUNT 3

void treeprint(struct bst_tok_node* root, int level) {
    if (root == NULL)
        return;
    for (int i = 0; i < level; i++)
        printf(i == level - 1 ? "|-" : "  ");
    printf("%d\n", root->T->dtype);
    treeprint(root->left, level + 1);
    treeprint(root->right, level + 1);
}

void print2DUtil(struct bst_tok_node* root, int space) {
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", root->T->dtype);

    // Process left child
    print2DUtil(root->left, space);
}

void print2D(struct bst_tok_node* root) {
    // Pass initial space count as 0
    print2DUtil(root, 0);
}