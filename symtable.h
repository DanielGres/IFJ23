/**
 * @file symtable.h
 * @author Mário Mihál
 * @brief Header with functions for symtable operations
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYMTABLE_H
#define SYNTABLE_H

#include <stdbool.h>

#include "dyn_string.h"
#include "token.h"

typedef struct
{
    char *name;
    token_type type;
    bool func_declared;
    bool function;
    bool variable;
    bool var_declared;
    struct bst_node *funcTree;
    struct bst_node *left;
    struct bst_node *right;
} bst_node;

/**
 * @brief A function that checks if function name is used by build-in function
 *
 * @param str2 Name of function
 * @return true
 * @return false
 */
bool InArray(char *str2);

/**
 * @brief A function that checks if all functions were declared
 *
 * @param root pointer to symtable
 * @return true
 * @return false
 */
bool Search_BTree_Control(bst_node **root);

/**
 * @brief Returns true if variable exists, false if doesnt
 *
 * @param root pointer to symtable
 * @param name name of a variable
 * @return true
 * @return false
 */
bool Is_In_BTree(bst_node **root, char *name);

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

/**
 * @brief A function that returns pointer to a subtree of a function
 *
 * @param root pointer to symtable
 * @param name name of the function
 * @return pointer to subtree
 */
struct bst_node *SubTreePointer(bst_node *root, char *name);

/**
 * @brief A function that prints all variables within the tree with a frame name.
 *
 * @param root pointer to symtable
 * @param GF True - GF False - LF
 */
void PrintAllVariablesinScope(bst_node **root, bool GF);

/**
 * @brief A function that inserts variable into function subtree
 *
 * @param root pointer to symtable
 * @param funcName
 * @param newName variable name
 * @param type variable type
 * @param declaration variable declared
 * @param var_type variable type
 * @return true
 * @return false
 */
bool Insert_FuncSubTree(bst_node **root, char *funcName, char *newName, token_type type, bool declaration, bool var_type);

#endif