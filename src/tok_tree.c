// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#include "../include/tok_tree.h"

void Init_TokTree(struct bst_tok_node *root) {
    (root) = NULL;
}
struct bst_tok_node *Set_TokNode(token *T_from) {
    struct bst_tok_node *newTokNode = malloc(sizeof(struct bst_tok_node));
    newTokNode->T = T_from;
    newTokNode->left = NULL;
    newTokNode->right = NULL;
    return newTokNode;
}

struct bst_tok_node *Insert_TokTree(struct bst_tok_node *root, bool isleft, token *T_from) {
    if (isleft) {
        root->left = Set_TokNode(T_from);
    } else {
        root->right = Set_TokNode(T_from);
    }
    return root;
}

void Ja(struct bst_tok_node *to, struct bst_tok_node *from)  // TODO ani za boha toto nenechajte JA
{
    *to = *from;
}

struct bst_tok_node *Create_TokTree(token *T_from, struct bst_tok_node *Left_root, struct bst_tok_node *Right_root) {
    struct bst_tok_node *root = Set_TokNode(T_from);
    root->right = Right_root;
    root->left = Left_root;

    return root;
}
struct bst_tok_node *SetChildNodes(struct bst_tok_node *root, struct bst_tok_node *Left_root, struct bst_tok_node *Right_root) {
    root->right = Right_root;
    root->left = Left_root;

    return root;
}

void inorderTraversal(struct bst_tok_node *root) {
    if (root == NULL) {
        return;
    } else {
        inorderTraversal((root->right));
        dynstr_print(root->T->val);
        inorderTraversal((root->left));
    }
}

void deleteTree(struct bst_tok_node *root) {
    if (root == NULL) {
        return;
    }

    free(root);
}