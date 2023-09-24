#ifndef TOKEN_H
#define TOKEN_H

#include "dyn_string.h"

/**
 * @brief types of token types
 * 
 */
typedef enum
{
    identifierT,// variable and function names
    intT,       // int
    doubleT,    // double
    stringT,    // string
    varT,       // var
    nilT,       // nil
    SbracketT,  // (
    EbracketT,  // )
    ErrT,       // error in lexer
    ifT,        // if
    elseT,      // else
    whileT,     // while
    letT,       // let
    returnT,    // return
    dotsT,      // :
    assignT,   // =
    funcT
} token_type;

typedef struct
{
    dyn_string *val;
    token_type dtype;
} token;

/**
 * @brief function that initializes the given Token
 * 
 * @param T Token to be initialized
 */
void Token_init(token **T);

/**
 * @brief function sets the Token's value and type 
 * 
 * @param T Token which value and type we want to set
 * @param string value attribute to be set on given Token
 * @param type  type attribute to be set on given Token
 */
void Token_set(token **T, dyn_string *string, token_type type);

#endif