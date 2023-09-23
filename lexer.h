#ifndef PARSER_H
#define PARSER_H


typedef enum
{
    START_STATE,

    ID_STATE,
    IDTYPE_STATE,
    LEFTBRACKET_STATE,
    RIGHTBRACKET_STATE,
    NUMBER_STATE,

} SM_STATE;

bool Get_Token(token **T);

bool lexer(dyn_string *buffer, token_type *type);

#endif