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

    while (condition)
    {
        if(!b_ex){
            c = fgetc(stdin);
        }
        //printf("'%c' is '%d'\n",c,c);
        switch(eNextState){
            case START_STATE:
            {
                
                if(c == '_' || (c >= 'a' && c <= 'z')){
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
                if(c == '_' || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
                    eNextState = ID_STATE;
                }
                else{
                    *type = variableT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }

            }
            break;
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
                else{
                    *type = intTypeT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            }
            break;
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
    }
    return true;
}