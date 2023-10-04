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
    if(myToken->dtype == letT){                                                 // "let" Rule
        Get_Token(&myToken);                                                    // looks for identifier token
        if(myToken->dtype != identifierT) return false;                         // checks if correct
        Get_Token(&myToken);                                                    // looks for double dots token
        if(myToken->dtype != dotsT) return false;                               // checks if correct
        Get_Token(&myToken);                                                    // looks for type
        if(myToken->dtype != intT && myToken->dtype != stringT) return false;   // check if any of int, double, string
        Get_Token(&myToken);                                                    // looks for assign sign token
        if(myToken->dtype != assignT) return false;                             // checks if correct
        Get_Token(&myToken);                                                    // looks for expression start
        if(expression()) return true;                                           // calls expression rule
    }
    else if(myToken->dtype == identifierT){                                     // "assign" Rule
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

bool CorpusPrime(){
    Get_Token(&myToken);
    switch(myToken->dtype){
        case ifT:
        {

        }
        break;
        case funcT:
        {

        }
        break;
        case varidT:
        {

        }
        break;
        case whileT:
        {

        }
        break;
        default:
        {
            return false;
        }
        break;
    }
}

bool Corpus();

bool CorpusRet();

bool FuncCallDecision();

bool FuncInputParams();

bool NextFuncInputParams();

bool ReturnF();

bool FuncVoidDecision();

bool ParamList();

bool NextParamList();

bool IfDecision();

bool IfElseDecision();

bool DeclarationArgs();

bool FinalDeclaration();

bool Declaration();

bool Exitus();
