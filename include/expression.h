#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "bin_tree.h"
#include "symtable.h"
#include "token.h"
#include ""

#define T_SIZE 9

typedef enum
{
    I_PLUS_MINUS, // 1 +-
    I_MUL_DIV,    // 0 */
    I_BRACKETS,   // 2 (
    I_BRACKETE,   // 3 )
    I_DATA,       // 4 id
    I_EN_OP,      // 5 equal/not equal
    I_LG_OP,      // 6 less/greates
    I_DOLLAR      // 7 dolar proste
} table_index_enum;

typedef enum
{
    S, // < SHIFT
    R, // > REDUCE
    E, // # ERROR/EMPTY
    N  // = EQUAL
} table_sign_enum;

typedef enum
{
 IF_CBracket, // IF starting with Brackets, ending with '{'
 IF_Let,      // IF starting with let, ending with '{'
 Function,    // Function, ending with ','/')' 
 Declaration  // Declaration, ending with Error or ';'
} expression_case;

/**
 * @brief boolean function checks if the next given token is a valid expression
 *
 * @param condition changes if current symbol is either "$" sign or brackete which sets the condition variable to 1 for ";" or 2 for brackete
 * @param root link to the table of symbols
 * @return true if the token is an expression
 * @return false if it does not fulfill any of the "if" or "switch" conditions
 */
bool Expression(int *condition, bst_node **root, int isCallLater, struct bst_tok_node *seed);

#endif