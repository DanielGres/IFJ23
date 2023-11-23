#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "symtable.h"
#include "tok_tree.h"
#include "token.h"

#define T_SIZE 10

// TO~DELETE
// typedef enum
// {
//     I_NOTNIL,       // 0 !
//     I_MUL_DIV,      // 1 */
//     I_PLUS_MINUS,   // 2 +-
//     I_EN_OP,        // 3 == != < > <= >=
//     I_DBLQST,       // 4 ??
//     I_BRACKETS,     // 5 (
//     I_BRACKETE,     // 6 )
//     I_DATA,         // 7 id
//     I_DOLLAR        // 8 dollar
// } table_index_enum;

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
 * @return true if the token is an expression
 * @return false if it does not fulfill any of the "if" or "switch" conditions
 */
bool Expression(struct bst_tok_node **seed, char *EOE);

void T_Body(stack *my_stack);

#endif