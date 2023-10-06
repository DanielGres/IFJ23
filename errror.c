// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errror.h"
#include "dyn_string.h"

/*
UPCOMING FUNCTION
errmessage errmess_set(char *txt, int num);
*/
//TODO UPRAVIT PRINTY

void mistake_print(dyn_string *msg, int errType)
{
    switch (errType)
    {
    case 1:
    {
        fprintf(stderr, "\nLEXICAL ERROR, %s\n", msg->s);
        // printf("\nLEXICAL ERROR, %s\n", msg->s);
    }
    break;
    case 2:
    {
        // fprintf(stderr, "\nSYNTAX ERROR");
        printf("\nSYNTAX ERROR");
    }
    break;
    case 3:
    {
        printf("\nSEMANTIC ERROR - not defined function OR occurrence of function redefinition");
        // fprintf(stderr, "\nSEMANTIC ERROR - not defined function OR occurrence of function redefinition");
    }
    break;
    case 4:
    {
        printf("\nSEMANTIC/RUNNING ERROR - incorrect number/types of parameters while calling function OR incorrect return type of function");
        // fprintf(stderr, "\nSEMANTIC/RUNNING ERROR - incorrect number/types of parameters while calling function OR incorrect return type of function");
    }
    break;
    case 5:
    {
        printf("\nSEMANTIC ERROR - occurrence of not defined variable use");
    }
    break;
    case 6:
    {
        printf("\nSEMANTIC/RUNNING ERROR - missing/redundant expression in function return command");
    }
    break;
    case 7:
    {
        printf("\nSEMANTIC/RUNNING ERROR - type compatibility in arithmetical, string or relation expressions");
    }
    break;
    case 8:
    {
        printf("\nSEMANTIC ERROR - some other type of semantical error");
    }
    break;
    case 99:
    {
        printf("\nINTERNAL COMPILATOR ERROR - error not affected by input program");
    }
    default:
    {
        printf("\nMAASKANTJE FOR THE WIN BITCH");
    }
    }
}
