#include <stdio.h>
#include "syntax.h"

token *myToken;

int main(){
    while(Get_Token(&myToken)){
        //dynstr_print((myToken->val));
        switch(myToken->dtype){
            case variableT:
            {
                printf("Variable\n");
            }
            break;
            case SbracketT:
            {
                printf("(\n");
            }
            break;
            case EbracketT:
            {
                printf(")\n");
            }
            break;
            case intTypeT:
            {
                printf("Integer\n");
            }
            break;
            default:
            {
                printf("strasny homosky GAY\n");
            }
        }
    }
}