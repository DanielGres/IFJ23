#ifndef TOKEN_H
#define TOKEN_H

#include "dyn_string.h"

/**
 * @brief types of token types
 * 
 */
typedef enum
{
    variableT,   //  $...
    operatorT,   //  +   -   *   / <  >  <=  >=  !==  === .
    assingmentT, //  =
    commaT,      //  ,
    ifT,         // if keyword
    whileT,      // while keyword
    elseT,       // else keyword
    functionT,   // function keyword
    floatT,      // float keyword
    intT,        // int keyword
    nullT,       // null keyword
    returnT,     // return keyword
    stringT,     // string keyword
    voidT,       // void keyword
    endT,        //  ;
    colonT,      //  :
    strTypeT,    // string
    intTypeT,    // int
    floatTypeT,  // float
    SbracketT,   //  (
    EbracketT,   //  )
    SCbracketT,  //  {
    ECbracketT,  //  }
    dataTypeT,   // with ? and without
    LcomTypeT,   // line comment
    BcomTypeT,   // block comment
    MadagaskarT,
    BENGININGT
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