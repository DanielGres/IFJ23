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

typedef enum {
    IF_CBracket,  // IF starting with Brackets, ending with '{'
    IF_Let,       // IF starting with let, ending with '{'
    Function,     // Function, ending with ','/')'
    Declaration   // Declaration, ending with Error or ';'
} expression_case;

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

void T_Body(stack *my_stack);

#endif