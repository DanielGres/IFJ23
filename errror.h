// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dyn_string.h"

typedef struct
{
    char *text;
    int num;
} errmessage;

// /** todo vymazat asi
//  * @brief function
//  *
//  * @param txt
//  * @param num specific error number
//  * @return errmessage
//  */
// void errmess_set(char *txt, int num);

/**
 * @brief function prints out on stderr where exactly error occurred
 *
 * @param msg error message, dynamic string expected
 * @param errType return number of the specific error
 */
void errPrint(dyn_string *msg, int errType);

#endif