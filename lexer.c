#include <stdio.h>
#include <stdbool.h>
#include "dyn_string.h"
#include "token.h"
#include "lexer.h"


bool stop = false;
bool b_ex = false;
char c = 'a';

bool Get_Token(token **T)
{
    if (!stop)
    {
        dyn_string buffer;
        token_type tdt;
        Token_init(T);
        dynstr_init(&buffer);
        if (lexer(&buffer, &tdt))
        {
            Token_set(T, &buffer, tdt);
            return true;
        }
        else
        {
            //mistake_print(&errMSG, errType);
            return false;
        }
    }
    return false;
}

bool lexer(dyn_string *buffer, token_type *type)
{
    SM_STATE eNextState = START_STATE;

    bool condition = true;
    bool compareWithTable = false;

    while (condition)
    {
        if(!b_ex){
            c = fgetc(stdin);
        }
        //printf("'%c' is '%d'\n",c,c);
        switch(eNextState){
            case START_STATE:
            {
                
                if(c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                    b_ex = false;
                    eNextState = ID_STATE;
                }
                else if(c == '('){
                    b_ex = false;
                    eNextState = LEFTBRACKET_STATE;
                }
                else if(c == ')'){
                    b_ex = false;
                    eNextState = RIGHTBRACKET_STATE;
                }
                else if(c >= '0' && c <= '9'){
                    b_ex = false;
                    eNextState = NUMBER_STATE;
                }
                else if(c == '"'){
                    b_ex = false;
                    eNextState = STRING1_STATE;
                }
                else if(c == ':'){
                    *type = dotsT;
                    condition = false;
                    b_ex = false;
                    eNextState = START_STATE;
                }
                else if(c == '='){
                    *type = assignT;
                    condition = false;
                    b_ex = false;
                    eNextState = START_STATE;
                }
                else if(c == 32 || c == 10){
                    b_ex = false;
                    eNextState = START_STATE;
                }
                else if(c == EOF){
                    return false;
                }
                
            }
            break;
            case ID_STATE:
            {
                if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
                    eNextState = ID2_STATE;
                }
                else{

                    *type = identifierT;
                    condition = false;
                    compareWithTable = true;
                    b_ex = true;
                    eNextState = START_STATE;
                }

            }
            break;
            case ID2_STATE:
            {
                if(c == '_' || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
                    eNextState = ID2_STATE;
                }
                else if(c == '?'){
                    IDTYPE_STATE;
                }
                else{
                    *type = identifierT;
                    condition = false;
                    compareWithTable = true;
                    b_ex = true;
                    eNextState = START_STATE;
                }

            }
            break;
            case IDTYPE_STATE:
            {
                *type = identifierT;
                condition = false;
                compareWithTable = true;
                b_ex = true;
                eNextState = START_STATE;
            }
            case LEFTBRACKET_STATE:
            {
                *type = SbracketT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
            break;
            case RIGHTBRACKET_STATE:
            {
                *type = EbracketT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
            break;
            case NUMBER_STATE:
            {
                if(c >= '0' && c <= '9'){
                    eNextState = NUMBER_STATE;
                }
                else if(c == '.'){
                    eNextState = DECNUMBER1_STATE;
                }
                else if(c == 'e' || c == 'E'){
                    eNextState = EXPNUMBER1_STATE;
                }
                else{
                    *type = intT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            }
            break;
            case DECNUMBER1_STATE:
            {
                if(c >= '0' && c <= '9'){
                    eNextState = DECNUMBER2_STATE;
                }
                else{
                    *type = ErrT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            }
            break;
            case DECNUMBER2_STATE:
            {
                if(c >= '0' && c <= '9'){
                    eNextState = DECNUMBER2_STATE;
                }
                else if(c == 'e' || c == 'E'){
                    eNextState = EXPNUMBER1_STATE;
                }
                else{
                    *type = doubleT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            }
            break;
            case EXPNUMBER1_STATE:
            {
                if(c >= '0' && c <= '9'){
                    eNextState = EXPNUMBER3_STATE;
                }
                else if(c == '+' || c == '-'){
                    eNextState = EXPNUMBER2_STATE;
                }
                else{
                    *type = ErrT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            }
            break;
            case EXPNUMBER2_STATE:
            {
                if(c >= '0' && c <= '9'){
                    eNextState = EXPNUMBER3_STATE;
                }
                else{
                    *type = ErrT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            }
            break;
            case EXPNUMBER3_STATE:
            {
                if(c >= '0' && c <= '9'){
                    eNextState = EXPNUMBER3_STATE;
                }
                else{
                    *type = doubleT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            }
            break;
            case STRING1_STATE:
            {
                if(c == 92){
                    eNextState = STRING2_STATE;
                }
                else if(c == "'"){
                    eNextState = STRING4_STATE;
                }
                else if(c >= 31){
                    eNextState = STRING3_STATE;
                }
            }
            default:
            {
                condition = false;
                return false;
            }
        }
        if (eNextState != START_STATE)
        {
            dynstr_add(buffer, c);
            
        }
        else{
            if(compareWithTable){
                if (dynstr_cmp(buffer, "if"))
                {
                    *type = ifT;
                }
                else if (dynstr_cmp(buffer, "while"))
                {
                    *type = whileT;
                }
                else if (dynstr_cmp(buffer, "else"))
                {
                    *type = elseT;
                }
                else if (dynstr_cmp(buffer, "func"))
                {
                    *type = funcT;
                }
                else if (dynstr_cmp(buffer, "Double"))
                {
                    *type = doubleT;
                }
                else if (dynstr_cmp(buffer, "Int"))
                {
                    *type = intT;
                }
                else if (dynstr_cmp(buffer, "nil"))
                {
                    *type = nilT;
                }
                else if (dynstr_cmp(buffer, "return"))
                {
                    *type = returnT;
                }
                else if (dynstr_cmp(buffer, "String"))
                {
                    *type = stringT;
                }
                else if (dynstr_cmp(buffer, "var"))
                {
                    *type = varT;
                }
                else if (dynstr_cmp(buffer, "let"))
                {
                    *type = letT;
                }
            }
        }
    }
    return true;
}