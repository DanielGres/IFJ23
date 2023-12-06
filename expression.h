/**
 * @file expression.h
 * @author Mário Mihál
 * @brief 
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "symtable.h"
#include "tok_tree.h"
#include "token.h"

#define T_SIZE 12

typedef enum {
    S,  // < SHIFT
    R,  // > REDUCE
    E,  // # ERROR/EMPTY
    N,  // = EQUAL
    F   // Function call
} table_sign_enum;

/**
 * @brief boolean function checks if the next given token is a valid expression
 *
 * @param condition changes if current symbol is either "$" sign or brackete which sets the condition variable to 1 for ";" or 2 for brackete
 * @param root link to the table of symbols
 * @return if > 0 then [1 - int, 2 - float, 3 - string]
 * @return if < 0 then [-1 intNill, -2 floatNill, -3 stringNill]
 * @return 0 -> bad expression
 */
int Expression(struct bst_tok_node **seed, char *EOE, bst_node **sym_table);

/**
 * @brief The function that generates AST tree from top 3 nodes on the stack, and then pushes new node onto stack
 *
 * @param my_stack
 */
void T_Body(stack *my_stack);

#endif