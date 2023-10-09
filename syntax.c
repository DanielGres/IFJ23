#include <stdio.h>
#include "syntax.h"

token *myToken;

const char* enumers[] = {
    "varidT",        // variable and function names
    "operatorT",     // !  * /  + -  == != < >  <= >=  ??
    "colonT",        // :
    "vartypeT",      // Double, Int, String or same with ? on the end
    "equalT",        // =
    "intnumT",      // whole number
    "doublemumT",   // double/float number
    "varT",          // var keyword
    "letT",          // let keyword
    "ifT",           // if keyword
    // "doubleT",       // Double keyword
    // "intT",          // Int keyword
    "stringT",       // string in the input code
    "nilT",          // nil keyword
    "LbracketT",     // (
    "RbracketT",     // )
    "LCbracketT",    // {
    "RCbracketT",    // }
    "elseT",         // else keyword
    "funcT",         // func keyword
    "paramNameT",    // name of the parameter in a function
    "commaT",        // ,
    "arrowT",        // ->
    "whileT",        // while keyword
    "returnT",       // return keyword
    "termT",         // TODO realne hocico to moze byt
    "newlineT",      // \n
    "semicolonT",    // ;
    "linecommentT",  // //comment
    "blockcommentT" //  /* comment */
};

int main(){
    // while(Get_Token(&myToken)){
    //     //dynstr_print((myToken->val));
    //     printf(" %s\n",enumers[myToken->dtype]);
        
    // }
    if(CorpusPrime()){
        printf("spravne! :DD\n");
    }
    else{
        printf("nespravne! :((\n");
    }
}



bool expression(){
    Get_Token(&myToken);
    if(myToken->dtype != intnumT) return false;
    return true;
}

bool CorpusPrime(){
    Get_Token(&myToken);
    switch(myToken->dtype){
        case ifT:
        {

        }
        break;
        case varT:
        {
            Get_Token(&myToken);
            if(myToken->dtype != varidT) return false;
            return DeclarationArgs();
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

bool DeclarationArgs(){
    Get_Token(&myToken);
    switch(myToken->dtype){
        case colonT:
        {
            Get_Token(&myToken);
            if(myToken->dtype != vartypeT) return false;
            return FinalDeclaration();
        }
        break;
        case equalT:
        {
            //return Funccalldecs..
            return false;
        }
        default:
        {
            return false;
        }
        break;
    }
}

bool FinalDeclaration(){
    Get_Token(&myToken);
    switch(myToken->dtype){
        case equalT:
        {
            if(!expression()) return false;
            return Exitus();
        }
        break;
        default:
        {
            return ExitusW();
        }
        break;
    }
}

bool Declaration();

bool Exitus(){
    Get_Token(&myToken);
    switch(myToken->dtype){
        case newlineT:
        {
            return true;
        }
        break;
        case semicolonT:
        {
            return true;
        }
        break;
        default:
        {
            return false;
        }
        break;
    }
}

bool ExitusW(){
    switch(myToken->dtype){
        case newlineT:
        {
            return true;
        }
        break;
        case semicolonT:
        {
            return true;
        }
        break;
        default:
        {
            return false;
        }
        break;
    }
}
