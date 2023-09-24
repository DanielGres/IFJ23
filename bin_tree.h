#ifndef BIN_TREE_H
#define BIN_TREE_H

#include <stdbool.h>
#include <stdlib.h>
#include "token.h"

struct bst_tok_node
{
    token *T;
    struct bst_tok_node *left;  // left node
    struct bst_tok_node *right; // right node
};

/**
 * @brief function initializes token BST with the given token 
 * 
 * @param root starting node
 */
void Init_TokTree(struct bst_tok_node *root);

/**
 * @brief function sets the new tokens left and right nodes to NULL value
 * 
 * @param T_from parameter from which are left and right nodes set to NULL value
 * @return struct bst_tok_node* root with left or right node set to NULL
 */
struct bst_tok_node *Set_TokNode(token *T_from);

/**
 * @brief function inserts either left or right node in case the boolean isleft is set to true or false
 * 
 * @param root given token node from which is next node (left/right) created
 * @param isleft boolean if true left, else right
 * @param T_from if the boolean isleft is either true or false, function Set_TokNode is called with this parameter
 * @return struct bst_tok_node* 
 */
struct bst_tok_node *Insert_TokTree(struct bst_tok_node *root, bool isleft, token *T_from);

/**
 * @brief function sets the DST as SRC 
 * 
 * @param to DST
 * @param from SRC
 */
void Ja(struct bst_tok_node *to, struct bst_tok_node *from);

/**
 * @brief function creates tree from the given first parameter and inserts left and right root from the next 2 given parameters
 * 
 * @param T_from start root node from which are left and right root inserted
 * @param Left_root parameter to be inserted as a left root of the given SRC token (1st parameter)
 * @param Right_root parameter to be inserted as a right root of the given SRC token (1st parameter)
 * @return struct bst_tok_node* 
 */
struct bst_tok_node *Create_TokTree(token *T_from, struct bst_tok_node *Left_root, struct bst_tok_node *Right_root);

/**
 * @brief inorder reading from the given token node
 * 
 * @param root starting node for reading
 */
void inorderTraversal(struct bst_tok_node *root);

/**
 * @brief function deletes the whole token BST
 *
 * @param node parameter to be freed 
 */
void deleteTree(struct bst_tok_node *node);

#endif