#include <stdio.h>
#include "syntax.h"

token *myToken;
bool TakeToken = true;
#define GetToken() if (TakeToken) { Get_Token(&myToken);} else {TakeToken = true;}

const char* enumers[] = {
    "varidT",        // variable and function names
    "operatorT",     // !  * /  + -  == != < >  <= >=  ??
    "colonT",        // :
    "vartypeT",      // Double, Int, String 
    "vartypeQT",     // Double?, Int?, String?
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
    //     dynstr_print((myToken->val));
    //     printf(" %s\n",enumers[myToken->dtype]);
        
    // }
    if(CorpusPrime()){
        printf("spravne! :DD\n");
    }
    else{
        printf("nespravne! :((\n");
    }
}



bool Expression(){
    GetToken();
    if(myToken->dtype != intnumT) return false;
    return true;
}

bool CorpusPrime(){
    GetToken();
    switch(myToken->dtype){
        case letT:{
            if(!Let()) return false;
            return CorpusPrime();
        }
        case varT:{
            if(!Var()) return false;
            return CorpusPrime();
        }
        break;
        case ifT:{
            if(!IfPrime()) return false;
            return CorpusPrime();
        }
        break;
        case whileT:{
            if(!While()) return false;
            return CorpusPrime();
        }
        break;
        case eofT:
        {
            return true;
        }
        break;
        default:
        {
            return false;
        }

    }
}

bool CorpusSecondary(){
    GetToken();
    TakeToken = true;
    switch (myToken->dtype){
        case letT:{
            if(!Let()) return false;
            return CorpusSecondary();
        }
        break;
        case RCbracketT:{
            return true;
        }
        break;
        default:
        {
            return false;
        }
    }
}

bool Let(){
    GetToken();
    if(myToken->dtype != varidT) return false;
    GetToken();
    switch(myToken->dtype){
        case colonT:{
            GetToken();
            if(myToken->dtype != vartypeT) return false;
            GetToken();
            if(myToken->dtype == equalT){
                return Expression();
            }
            else{
                TakeToken = false;
                return true;
            }
        }
        break;
        case equalT:{
            return Expression();
        }
        break;
        default:{
            return false;
        }  
    }

}

bool Var(){
    GetToken();
    if(myToken->dtype != varidT) return false;
    GetToken();
    switch(myToken->dtype){
        case colonT:{
            GetToken();
            if(myToken->dtype != vartypeT) return false;
            GetToken();
            if(myToken->dtype == equalT){
                return Expression();
            }
            else{
                TakeToken = false;
                return true;
            }
        }
        break;
        case equalT:{
            return Expression();
        }
        break;
        default:{
            return false;
        }  
    }

}

bool IfPrime(){
    if(!Expression()) return false;
    GetToken();
    if(myToken->dtype != LCbracketT) return false;
    if(!CorpusSecondary()) return false;
    GetToken();
    if(myToken->dtype != elseT) return false;
    GetToken();
    if(myToken->dtype != LCbracketT) return false;
    if(!CorpusSecondary()) return false;
    return true;
}

bool While(){
    if(!Expression()) return false;
    GetToken();
    if(myToken->dtype != LCbracketT) return false;
    if(!CorpusSecondary()) return false;
    return true;
}