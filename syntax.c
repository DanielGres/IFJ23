// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#include "syntax.h"

token *myToken;
bool TakeToken = true;
#define GetToken() if (TakeToken) { Get_Token(&myToken);} else {TakeToken = true;}


bool Expression(){
    GetToken();
    if(myToken->dtype != intnumT) return false;
    return true;
}

bool CorpusPrime(struct bst_tok_node **seed){
    GetToken();
    switch(myToken->dtype){
        case letT:{
            *seed = Set_TokNode(myToken);
            if(!Let(&((*seed)->left))) return false;
            if(!EndCommand()) return false;
            return CorpusPrime(&((*seed)->right));
        }
        case ifT:{
            *seed = Set_TokNode(myToken);
            if(!IfPrime(&((*seed)->left))) return false;
            return CorpusPrime(&((*seed)->right));
        }
        break;
        case eofT:
        {
            return true;
        }
        break;
        case newlineT:
        {
            return CorpusPrime(&(*seed));
        }
        break;
        default:
        {
            return false;
        }

    }
}

bool CorpusSecondary(struct bst_tok_node **seed){
    GetToken();
    switch (myToken->dtype){
        case letT:{
            *seed = Set_TokNode(myToken);
            if(!Let(&((*seed)->left))) return false;
            if(!EndCommand()) return false;
            return CorpusSecondary(&((*seed)->right));
        }
        break;
        case RCbracketT:{
            return true;
        }
        break;
        case newlineT:
        {
            return CorpusSecondary(&((*seed)));
        }
        break;
        default:
        {
            return false;
        }
    }
}

bool EndCommand(){
    GetToken();
    switch(myToken->dtype){
        case semicolonT:{
            GetToken();
            if(myToken->dtype == newlineT) return true;
            else{
                TakeToken = false;
                return true;
            }
        }
        break;
        case newlineT:{
            return true;
        }
        break;
        case eofT:
        {
            TakeToken = false;
            return true;
        }
        break;
        default:{
            return false;
        }
    }
}

void EnterSkip(){
    if(myToken->dtype == newlineT){
        GetToken();
    }
}

bool Let(struct bst_tok_node **seed){
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

bool IfPrime(struct bst_tok_node **seed){
    if(!Expression()) return false;
    GetToken();
    EnterSkip();
    if(myToken->dtype != LCbracketT) return false;
    *seed = Set_TokNode(myToken);
    if(!CorpusSecondary(&((*seed)->left))) return false;
    GetToken();
    EnterSkip();
    if(myToken->dtype != elseT) return false;
    GetToken();
    EnterSkip();
    if(myToken->dtype != LCbracketT) return false;
    if(!CorpusSecondary(&((*seed)->left))) return false;
    return true;
}
