#ifndef PARSER_H
#define PARSER_H


typedef enum
{
    START_STATE,

    ID_STATE,
    ID2_STATE,
    IDTYPE_STATE,
    LEFTBRACKET_STATE,
    RIGHTBRACKET_STATE,
    NUMBER_STATE,
    DECNUMBER1_STATE,
    DECNUMBER2_STATE,
    EXPNUMBER1_STATE,
    EXPNUMBER2_STATE,
    EXPNUMBER3_STATE,
    STRING1_STATE,
    STRING2_STATE,
    STRING3_STATE,
    STRING4_STATE

} SM_STATE;

bool Get_Token(token **T);

bool lexer(dyn_string *buffer, token_type *type);

#endif