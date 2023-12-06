// Implementace překladače imperativního jazyka IFJ23.


/**
 * @file codegen.h
 * @authors Daniel Greš
 * @brief 
 * @version 0.1
 * @date 2023-12-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CODEGEN_H
#define CODEGEN_H

#include "expression.h"
#include "tok_tree.h"
#include "token.h"

/**
 * @brief Function that generates all built-in functions
 * 
 */
void Instructions();

/**
 * @brief Function rides along right side of the tree and generates code for each node
 * 
 */
void GenerateSubTree(struct bst_tok_node *curr_root);

/**
 * @brief Function rides along right side of the tree and generates code for each node inside function, added return support
 * 
 */
void GenerateSubTreeFunction(struct bst_tok_node *curr_root);

/**
 * @brief Creates segment for function definition
 * 
 * @param root 
 */
void GenerateFunctionDefinition(struct bst_tok_node *root);

/**
 * @brief Converts passed parameters into usable form
 * 
 * @param root 
 */
void PrepareFuncParams(struct bst_tok_node *root);


/**
 * @brief Function decides if it's assigment case or call to function case
 * 
 * @param root 
 * @param inFunction 
 */
void CallFuncAssigment(struct bst_tok_node *root);

/**
 * @brief This function correctly passes parameters onto temporary frame
 * 
 * @param root 
 */
void PrepareFuncCallParams(struct bst_tok_node *root);

/**
 * @brief Function decides which function to call
 * 
 * @param root 
 */
void GenerateCallFunction(struct bst_tok_node *root);

/**
 * @brief Calls for built in length function
 * 
 * @param root 
 */
void GenerateLength(struct bst_tok_node *root);

/**
 * @brief Calls for built in conversion function;
 * 
 * @param root 
 */
void GenerateInt2Double(struct bst_tok_node *root);

/**
 * @brief Calls for built in conversion function;
 * 
 * @param root 
 */
void GenerateDouble2Int(struct bst_tok_node *root);

/**
 * @brief Calls for reading functions
 * 
 * @param root 
 */
void GenerateCallReadString(struct bst_tok_node *root);
void GenerateCallReadInt(struct bst_tok_node *root);
void GenerateCallReadFloat(struct bst_tok_node *root);

/**
 * @brief Iterativly calls for write function as this function can have infinite parameters
 * 
 * @param root 
 */
void GenerateCallWrite(struct bst_tok_node *root);

/**
 * @brief Calls just for expression to be generated and result will be accesible in GF@exp
 * 
 * @param root 
 * @param inFunction 
 */
void GenerateAssigment(struct bst_tok_node *root);

/**
 * @brief Main function that generates helper variables, built-in functions and main code
 * 
 * @param root 
 * @param tableOfSymbols 
 */
void Generator(struct bst_tok_node *root, bst_node **tableOfSymbols);

/**
 * @brief Function correctly modifies int and float values so they can be used in expressions
 * 
 */
void convertToFloatAndSwap();

/**
 * @brief Special case for + operation, acts as concatenation for strings
 * 
 */
void checkString();

/**
 * @brief Creates whole expression for interpret to run correctly
 * 
 * @param root 
 * @param inFunction 
 */
void GenerateExprInstruction(struct bst_tok_node *root);

/**
 * @brief Traverses expression tree correct way
 * 
 * @param root 
 * @param inFunction 
 */
void ExpressionPostorderTraversal(struct bst_tok_node *root);

/**
 * @brief Calls for expression creation
 * 
 * @param root 
 * @param inFunction 
 */
void GenerateExpression(struct bst_tok_node *root);

/**
 * @brief Creates variable for interpret and assings value to it if needed, otherwise nil will be initialised
 * 
 * @param root 
 * @param inFunction 
 */
void GenerateLet(struct bst_tok_node *root);
void GenerateVar(struct bst_tok_node *root);

/**
 * @brief Prepares while cycle for interpret
 * 
 * @param root 
 * @param inFunction 
 */
void GenerateWhile(struct bst_tok_node *root);

/**
 * @brief Prepares if statement for interpret
 * 
 * @param root 
 * @param inFunction 
 */
void GenerateIF(struct bst_tok_node *root);

#endif