#ifndef TOKEN_H
#define TOKEN_H

#include "dyn_string.h"

/**
 * @brief types of token types
 * 
 */
typedef enum
{
    varidT, // variable and function names
    operatorT, // !  * /  + -  == != < >  <= >=  ??
    colonT, // :
    vartypeT, //  Double, Int, String or same with ? on the the end 
    equalT, // = 
    vyrazT, 
    intnumT, // whole number
    doublenumT, // double/float number
    varT, // var keyword
    letT, // let keyword
    ifT, // if keyword
    // doubleT, // Double keyword NETREBA JE TO VARTYPE
    // intT, // Int keyword NETREBA JE TO V VARTYPE
    stringT, // string in the input code
    nilT, // nil keyword
    LbracketT, // (
    RbracketT, // )
    LCbracketT, // {
    RCbracketT, // }
    elseT, // else keyword
    funcT, // func keyword
    paramNameT, // name of parameter in function TODO no idea how toto rozlisit asi az v syntaxi/semantike
    commaT, // ,
    arrowT, // ->
    whileT, // while keyword
    returnT, // return keyword
    termT, // TODO realne hocico to moze byt
    newlineT, // \n //TODO probably iba ukoncenie riadku netreba token na toto asi
    semicolonT, // ; //TODO probably iba ukoncenie riadku netreba token na toto asi
    linecommentT, //comment
    blockcommentT, /* comment */
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