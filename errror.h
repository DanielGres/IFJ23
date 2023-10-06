#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include "dyn_string.h"

typedef struct
{
    char *text;
    int num;
} errmessage;

/**
 * @brief function 
 * 
 * @param txt 
 * @param num specific error number
 * @return errmessage 
 */
void errmess_set(char *txt, int num);

/**
 * @brief function prints out on stderr where exactly error occurred
 * 
 * @param msg error message, dynamic string expected 
 * @param errType penis
 */
void mistake_print(dyn_string *msg, int errType);

#endif