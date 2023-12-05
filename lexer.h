// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "dyn_string.h"
#include "token.h"

/**
 * @brief Enumerated type for the states of the finite state machine
 * 
 */
typedef enum {
    START_STATE,

    ID_STATE_0,
    ID_STATE,
    ID2_STATE,
    IDTYPE_STATE,
    LEFTBRACKET_STATE,
    RIGHTBRACKET_STATE,
    RIGHTCURLYBRACKET_STATE,
    LEFTCURLYBRACKET_STATE,
    NUMBER_STATE,
    DECNUMBER1_STATE,
    DECNUMBER2_STATE,
    EXPNUMBER1_STATE,
    EXPNUMBER2_STATE,
    EXPNUMBER3_STATE,
    STRING_STATE,
    STRING2_STATE,
    STRING3_STATE,
    STRINGESCAPE_STATE,
    STRINGU_STATE,
    STRINGU2_STATE,
    STRINGU3_STATE,
    STRINGMULTI_STATE,
    STRINGMULTI2_STATE,
    STRINGMULTI3_STATE,
    STRINGMULTI4_STATE,
    STRINGMULTI5_STATE,
    STRINGMULTI6_STATE,
    STRINGMULTI7_STATE,
    STRINGMULTI8_STATE,
    STRINGMULTI9_STATE,
    COLON_STATE,
    PLUS_STATE,
    MINUS_STATE,
    ARROW_STATE,
    COMPARE_STATE,
    EXCLAMATION_STATE,
    EQUAL_STATE,
    NOTEQUAL_STATE,
    MULTIP_STATE,
    SLASH_STATE,
    LINECOMM_STATE,
    LINECOMM2_STATE,
    BLOCKCOMM_STATE,
    BLOCKCOMM2_STATE,
    BLOCKCOMM3_STATE,
    GREATER_STATE,
    GREATEREQUAL_STATE,
    LESS_STATE,
    LESSEQUAL_STATE,
    QUESTION_STATE,
    QUESTION2_STATE,
    COMMA_STATE,
    SEMICOL_STATE,
    NEWLINE_STATE,
    AND_STATE,
    AND2_STATE,
    OR_STATE,
    OR2_STATE,
} SM_STATE;


/**
 * @brief Function to initialize the token with the parsed value and type from lexer
 * 
 * @param T token which will be initialized with the parsed correct value and type
 */
bool Get_Token(token **T);

/**
 * @brief Function to convert a single hexadecimal character to an integer
 * 
 * @param c hexadecimal character to be converted
 */
int hexCharToInt(char c);

/**
 * @brief Function to convert a hexadecimal string to its decimal representation
 * 
 * @param hex hexadecimal string to be converted
 */
int hexToDecimal(dyn_string *hex);

/**
 * @brief // Function that replaces unicode sequences in the input string with their corresponding ASCII characters and also replaces escape sequences
 * 
 * @param inputString string to be processed
 */
void replaceUnicodeSequences(dyn_string *inputString);

/**
 * @brief Function to convert scientific notation to floating point decimal notation
 * 
 * @param str string to be processed
 */
void dynstr_sci_to_dec(dyn_string *str);

/**
 * @brief function that parses the input code and finds the token type and value
 * 
 * @param buffer found tokens value will be stored here
 * @param type found tokens type will be stored here
 */
bool lexer(dyn_string *buffer, token_type *type);

#endif