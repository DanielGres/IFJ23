#include <stdio.h>
#include "syntax.h"

token *myToken;

const char* enumers[] = {
    "identifierT",
    "intT",
    "doubleT",
    "stringT",
    "varT",
    "nilT",
    "SbracketT",
    "EbracketT",
    "ErrT",
    "ifT",
    "elseT",
    "whileT",
    "letT",
    "returnT",
    "funcT"
};

int main(){
    // while(Get_Token(&myToken)){
    //     //dynstr_print((myToken->val));
    //     printf("%s\n",enumers[myToken->dtype]);
        
    // }
    if(main_body()){
        printf("spravne! :DD\n");
    }
    else{
        printf("nespravne! :((\n");
    }
}

bool main_body(){
    Get_Token(&myToken);
    if(myToken->dtype == letT){
        Get_Token(&myToken);
        if(myToken->dtype != identifierT) return false;
        Get_Token(&myToken);
        if(myToken->dtype != dotsT) return false;
        Get_Token(&myToken);
        if(myToken->dtype != intT && myToken->dtype != stringT) return false;
        Get_Token(&myToken);
        if(myToken->dtype != assignT) return false;
        Get_Token(&myToken);
        if(expression()) return true;    
    }
    return false;
}

bool expression(){
    //precedencna tabulka etc :))))
    return true;
}
