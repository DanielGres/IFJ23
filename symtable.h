#ifndef SYMTABLE_H
#define SYNTABLE_H

#include <stdbool.h>

#include "dyn_string.h"
#include "token.h"

// typedef struct
// {
//     int len;
//     char **list;
// } bst_list;

typedef struct
{
    char *name;
    token_type type;
    bool func_declared;
    bool function;
    bool variable;
    bool var_declared;
    struct bst_node *left;   // ľavý potomok
    struct bst_node *right;  // pravý potomok
} bst_node;

bool Search_BTree_Control(bst_node **root);

bool Is_In_BTree(bst_node **root,char *name);

/**
 * @brief function initializer BST to the table of symbols
 *
 * @param root main root from which the BST is created
 */
void Init_BTree(bst_node **root);

/**
 * @brief function inserts node either to the left or right from the given node root with given parameters
 *
 * @param root @param name @param type parameters for to be created note
 * @param declaration boolean if the node was declared
 * @param type
 * @return true
 * @return false as a default or if the root has value NULL
 */
bool Insert_BTree(bst_node **root, char *name, token_type type, bool declaration, bool var_type);

/**
 * @brief preorder read from the given node root
 *
 * @param root start of the preorder read
 */
void preorderTraversal(bst_node **root);

#endif