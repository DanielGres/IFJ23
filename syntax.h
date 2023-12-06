/**
 * @file syntax.h
 * @authors Daniel Greš, Mário Mihál
 * @brief 
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYNTAX_H
#define SYNTAX_H

#include "expression.h"
#include "lexer.h"
#include "tok_tree.h"

bool EndCommand();

void EnterSkip();

/**
 * @brief Main program block syntax analyser,
 *  recursively calls itself and other functions until end of file
 *
 * @param seed
 * @return true
 * @return false
 */
bool CorpusPrime(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Body rule for defining functions, insides of while and if statements
 * 
 */
bool CorpusSecondary(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Control for correct end of instruction
 * 
 * @return true 
 * @return false 
 */
bool EndCommand();


/**
 * @brief Rule for creating users functions
 * 
 * @param seed 
 * @param sym_table 
 * @return true 
 * @return false 
 */
bool FunctionDef(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Recursive rule for check of users parameters
 * 
 * @param seed 
 * @param sym_table 
 * @param name 
 * @return true 
 * @return false 
 */
bool FunctionDefParams(struct bst_tok_node **seed, bst_node **sym_table, char *name);

/**
 * @brief Call for expressions which also handle calling of functions
 * 
 */
bool assigment(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Used to check correct syntax of calling functions
 * 
 * @param seed 
 * @param sym_table 
 * @return true 
 * @return false 
 */
bool FunctionCall(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Rule for passing parameters
 * 
 * @param seed 
 * @param sym_table 
 * @return true 
 * @return false 
 */
bool FunctionCallParameters(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Rule for creating variables
 * 
 * @param seed 
 * @param sym_table 
 * @return true 
 * @return false 
 */
bool Let(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Rule for creating variables
 * 
 * @param seed 
 * @param sym_table 
 * @return true 
 * @return false 
 */
bool Var(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Rule for if statement
 * 
 * @param seed 
 * @param sym_table 
 * @return true 
 * @return false 
 */
bool IfPrime(struct bst_tok_node **seed, bst_node **sym_table);

/**
 * @brief Rule for while statement
 * 
 * @param seed 
 * @param sym_table 
 * @return true 
 * @return false 
 */
bool WhilePrime(struct bst_tok_node **seed, bst_node **sym_table);

#endif
