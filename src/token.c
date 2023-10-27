// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#include "../include/token.h"

void Token_init(token **T)
{
    (*T) = NULL;
}

void Token_set(token **T, dyn_string *string, token_type type)
{
    if ((*T) == NULL)
    {
        (*T) = malloc(sizeof(token));
        (*T)->dtype = type;
        (*T)->val = malloc(sizeof(dyn_string));
        dynstr_copy((*T)->val, string);
    }
    else
    {
        (*T)->dtype = type;
        (*T)->val = malloc(sizeof(dyn_string));
        dynstr_copy((*T)->val, string);
        return;
    }
}
