// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#ifndef DYN_STRING_H
#define DYN_STRING_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    char *s;            // string ending with /0
    unsigned int allen; // allocated length of string
    unsigned int len;   // length
} dyn_string;

/**
 * @brief Function inicialize dynamic allocated string
 *
 * @param str inicializes given string
 */
void dynstr_init(dyn_string *str);

/**
 * @brief Function pastes given character to the end of dynamic string
 *
 * @param str destination for character where it's going to be added
 * @param c character that's about to be added to the dynamic string
 */
void dynstr_add(dyn_string *str, char c);

/**
 * @brief function adds string from the second parameter to the first one
 * 
 * @param str destination dynamic string
 * @param strtoadd string to be added to the first argument
 */
void dynstr_addstr(dyn_string *str, char *strtoadd);

/**
 * @brief Function copies data from one dynamic string to another
 *
 * @param to copy to destination
 * @param from copy from source
 */
void dynstr_copy(dyn_string *to, dyn_string *from);

/**
 * @brief Bool function compares 2 given dynamic strings
 *
 * @param string1 first dynamic string to be compared
 * @param string2 second basic string to be compared
 * @return true statement in case of identical strings
 * @return false statement in case the strings are not identical
 */
bool dynstr_cmp(dyn_string *string1, char *string2);

/**
 * @brief Function clears given dynamic string
 *
 * @param str dynamic string to be cleared
 */
void dynstr_clear(dyn_string *str);

/**
 * @brief Function destroys given dynamic string
 *
 * @param str dynamic string to be freed
 */
void dynstr_destroy(dyn_string *str);

/**
 * @brief Function prints given dynamic string
 * 
 * @param str dynamic string to be printed
 */
void dynstr_print(dyn_string *str);

#endif