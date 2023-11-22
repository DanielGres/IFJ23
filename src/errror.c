// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#include "../include/errror.h"

/* todo vymazat asi
UPCOMING FUNCTION
errmessage errmess_set(char *txt, int num);
*/
// TODO UPRAVIT PRINTY

void errPrint(dyn_string *msg, int errType) {
    switch (errType) {
        case 1: {
            fprintf(stderr, "LEXICAL ERROR, %s\n", msg->s);
        } break;
        case 2: {
            fprintf(stderr, "SYNTAX ERROR, %s\n", msg->s);
        } break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:  // TODO takto to vypisovat a vsade v kode budes musiet nastavovat errType aj msg a volat tuto funkciu?
        {
            fprintf(stderr, "SEMANTIC ERROR, %s\n", msg->s);
        } break;
        case 9: {
            fprintf(stderr, "OTHER SEMANTIC ERROR, %s\n", msg->s);
        } break;
        case 99: {
            fprintf(stderr, "INTERNAL COMPILATOR ERROR - error not caused by input program\n");
        }
        default: {
            fprintf(stderr, "UNKNOWN ERROR\n");
        }
    }
}
