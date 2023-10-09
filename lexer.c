#include <stdio.h>
#include <stdbool.h>
#include "dyn_string.h"
#include "token.h"
#include "lexer.h"
#include "errror.h"

bool stop = false;
bool b_ex = false;
char c = 'a';
int stringCounter = 0;

// TODO errory
// int errType = 0;
// dyn_string errMSG;
// dynstr_init(errMSG);

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
            Token_set(T, &buffer, endT);
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
        if (!b_ex)
        {
            c = fgetc(stdin);
        }
        // printf("'%c' is '%d'\n",c,c);
        switch (eNextState)
        {
        case START_STATE:
        {

            if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                b_ex = false;
                eNextState = ID_STATE;
            }
            else if (c == '(')
            {
                b_ex = false;
                eNextState = LEFTBRACKET_STATE;
            }
            else if (c == ')')
            {
                b_ex = false;
                eNextState = RIGHTBRACKET_STATE;
            }
            else if (c == '{')
            {
                b_ex = false;
                eNextState = LEFTCURLYBRACKET_STATE;
            }
            else if (c == '}')
            {
                b_ex = false;
                eNextState = RIGHTCURLYBRACKET_STATE;
            }
            else if (c >= '0' && c <= '9')
            {
                b_ex = false;
                eNextState = NUMBER_STATE;
            }
            else if (c == '"')
            {
                b_ex = false;
                eNextState = STRING_STATE;
            }
            else if (c == ':')
            {
                b_ex = false;
                eNextState = COLON_STATE;
            }
            else if (c == '=')
            {
                b_ex = false;
                eNextState = EQUAL_STATE;
            }
            else if (c == '+')
            {
                b_ex = false;
                eNextState = PLUS_STATE;
            }
            else if (c == '-')
            {
                b_ex = false;
                eNextState = MINUS_STATE;
            }
            else if (c == '!')
            {
                b_ex = false;
                eNextState = EXCLAMATION_STATE;
            }
            else if (c == '*')
            {
                b_ex = false;
                eNextState = MULTIP_STATE;
            }
            else if (c == '/')
            {
                b_ex = false;
                eNextState = SLASH_STATE;
            }
            else if (c == '>')
            {
                b_ex = false;
                eNextState = GREATER_STATE;
            }
            else if (c == '<')
            {
                b_ex = false;
                eNextState = LESS_STATE;
            }
            else if (c == '?')
            {
                b_ex = false;
                eNextState = QUESTION_STATE;
            }
            else if (c == ',')
            {
                b_ex = false;
                eNextState = COMMA_STATE;
            }
            else if (c == ';')
            {
                b_ex = false;
                eNextState = SEMICOL_STATE;
            }
            // else if(c == 10)
            // {
            //     b_ex = false;
            //     eNextState = NEWLINE_STATE;
            // }
            else if (c == 32 || c == 10)
            { // Exitus
                b_ex = false;
                eNextState = START_STATE;
            }
            else if (c == EOF)
            {
                return false;
            }
            else // TODO errory
            {
                b_ex = false;
                condition = false;
                // dynstr_addstr(&errMSG, "expected a valid number, alphanumerical character, operator!\n");
                // errType = 1;
                return false;
            }
        }
        break;
        // case NEWLINE_STATE: // \n
        // {
        //     *type = newlineT;
        //     b_ex = true;
        //     condition = false;
        //     eNextState = START_STATE;
        // }
        // break;
        case SEMICOL_STATE: // ;
        {
            *type = semicolonT;
            b_ex = true;
            condition = false;
            eNextState = START_STATE;
        }
        break;
        case COMMA_STATE: // ,
        {
            *type = commaT;
            b_ex = true;
            condition = false;
            eNextState = START_STATE;
        }
        break;
        case QUESTION_STATE: // ?
        {
            if (c == '?')
            {
                b_ex = false;
                eNextState = QUESTION2_STATE;
            }
            else
            {
                // TODO error
                return false;
            }
        }
        break;
        case QUESTION2_STATE: // ??
        {
            *type = operatorT;
            b_ex = true;
            condition = false;
            eNextState = START_STATE;
        }
        break;
        case GREATER_STATE: // >
        {
            if (c == '=')
            {
                b_ex = false;
                eNextState = GREATEREQUAL_STATE;
            }
            else
            {
                *type = operatorT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            }
        }
        break;
        case LESS_STATE: // <
        {
            if (c == '=')
            {
                b_ex = false;
                eNextState = LESSEQUAL_STATE;
            }
            else
            {
                *type = operatorT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            }
        }
        break;
        case GREATEREQUAL_STATE: // >=
        {
            *type = operatorT;
            b_ex = true;
            condition = false;
            eNextState = START_STATE;
        }
        break;
        case LESSEQUAL_STATE: // <=
        {
            *type = operatorT;
            b_ex = true;
            condition = false;
            eNextState = START_STATE;
        }
        break;
        case SLASH_STATE: // /
        {
            if (c == '/')
            {
                eNextState = LINECOMM_STATE;
            }
            else if (c == '*')
            {
                eNextState = BLOCKCOMM_STATE;
            }
            else
            {
                *type = operatorT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            }
        }
        break;
        case LINECOMM_STATE: // //
        {
            if (c != '\n')
            {
                b_ex = false;
                eNextState = LINECOMM_STATE;
            }
            else if (c == '\n')
            {
                b_ex = false;
                eNextState = LINECOMM2_STATE;
            }
            else
            {
                // TODO error BOLO ZAKOMENTOVANE 
                return false;
            }
        }
        break;
        case LINECOMM2_STATE: // //comment
        {
            *type = linecommentT;
            b_ex = true;
            condition = false;
            eNextState = START_STATE;
        }
        break;
        case BLOCKCOMM_STATE: // /* comment
        {
            if (c != '*')
            {
                eNextState = BLOCKCOMM_STATE;
            }
            else if (c == '*')
            {
                b_ex = false;
                eNextState = BLOCKCOMM2_STATE;
            }
            else
            {
                // TODO error BOLO ZAKOMENTOVANE
                return false;
            }
        }
        break;
        case BLOCKCOMM2_STATE: // /* comment *
        {
            if (c == '/')
            {
                b_ex = false;
                eNextState = BLOCKCOMM3_STATE;
            }
            else if (c == '*')
            {
                b_ex = false;
                eNextState = BLOCKCOMM2_STATE;
            }
            else if (c != '*')
            {
                b_ex = false;
                eNextState = BLOCKCOMM_STATE;
            }
            else
            {
                // TODO error BOLO ZAKOMENTOVANE
                return false;
            }
        }
        break;
        case BLOCKCOMM3_STATE: // /* comment */
        {
            *type = blockcommentT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case MULTIP_STATE: // *
        {
            *type = operatorT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case MINUS_STATE: // -
        {
            if (c == '>')
            {
                b_ex = false;
                eNextState = ARROW_STATE;
            }
            else
            {
                *type = operatorT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case ARROW_STATE: // ->
        {
            *type = arrowT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case PLUS_STATE: // +
        {
            *type = operatorT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case COLON_STATE: // :
        {
            *type = colonT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case EQUAL_STATE: // =
        {
            if (c == '=')
            {
                b_ex = false;
                eNextState = COMPARE_STATE;
            }
            else
            {
                *type = equalT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case COMPARE_STATE: // ==
        {
            *type = operatorT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case EXCLAMATION_STATE: // !
        {
            if (c == '=')
            {
                b_ex = false;
                eNextState = NOTEQUAL_STATE;
            }
            else
            {
                *type = operatorT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case NOTEQUAL_STATE:
        {
            *type = operatorT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case ID_STATE:
        {
            if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            {
                b_ex = false;
                eNextState = ID2_STATE;
            }
            else
            {
                *type = varidT;
                condition = false;
                compareWithTable = true;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case ID2_STATE:
        {
            if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            {
                b_ex = false;
                eNextState = ID2_STATE;
            }
            else if (c == '?')
            {
                eNextState = IDTYPE_STATE;
            }
            else
            {
                *type = varidT;
                condition = false;
                compareWithTable = true;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case IDTYPE_STATE:
        {
            *type = varidT;
            condition = false;
            compareWithTable = true;
            b_ex = true;
            eNextState = START_STATE;
        }
        case LEFTBRACKET_STATE:
        {
            *type = LbracketT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case RIGHTBRACKET_STATE:
        {
            *type = RbracketT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case LEFTCURLYBRACKET_STATE:
        {
            *type = LCbracketT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case RIGHTCURLYBRACKET_STATE:
        {
            *type = RCbracketT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case NUMBER_STATE:
        {
            if (c >= '0' && c <= '9')
            {
                eNextState = NUMBER_STATE;
            }
            else if (c == '.')
            {
                eNextState = DECNUMBER1_STATE;
            }
            else if (c == 'e' || c == 'E')
            {
                eNextState = EXPNUMBER1_STATE;
            }
            else
            {
                *type = intnumT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case DECNUMBER1_STATE:
        {
            if (c >= '0' && c <= '9')
            {
                eNextState = DECNUMBER2_STATE;
            }
            else
            {
                // *type = ErrT; TODO
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case DECNUMBER2_STATE:
        {
            if (c >= '0' && c <= '9')
            {
                eNextState = DECNUMBER2_STATE;
            }
            else if (c == 'e' || c == 'E')
            {
                eNextState = EXPNUMBER1_STATE;
            }
            else
            {
                *type = doublenumT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case EXPNUMBER1_STATE:
        {
            if (c >= '0' && c <= '9')
            {
                eNextState = EXPNUMBER3_STATE;
            }
            else if (c == '+' || c == '-')
            {
                eNextState = EXPNUMBER2_STATE;
            }
            else
            {
                // *type = ErrT; TODO
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case EXPNUMBER2_STATE:
        {
            if (c >= '0' && c <= '9')
            {
                eNextState = EXPNUMBER3_STATE;
            }
            else
            {
                // *type = ErrT; TODO
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case EXPNUMBER3_STATE:
        {
            if (c >= '0' && c <= '9')
            {
                eNextState = EXPNUMBER3_STATE;
            }
            else
            {
                *type = doublenumT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            }
        }
        break;
        case STRING_STATE: // "
        {
            if (c == '\\')
            {
                b_ex = false;
                eNextState = STRINGESCAPE_STATE;
            }
            else if (c == '"')
            {
                b_ex = false;
                eNextState = STRINGMULTI_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRING2_STATE;
            }
        }
        break;
        case STRING2_STATE: // "something
        {
            if(c == '"')
            {
                b_ex = false;
                eNextState = STRING3_STATE;
            }
            else if (c == '\\')
            {
                b_ex = false;
                eNextState = STRINGESCAPE_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRING2_STATE;
            }
        }
        break;
        case STRING3_STATE:
        {
            *type = stringT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
        }
        break;
        case STRINGESCAPE_STATE: // "backslash(\)
        {
            if (c == 'u')
            {
                b_ex = false;
                eNextState = STRINGU_STATE;
            }
            else if (c == '"' || c == 'r' || c == 'n' || c == 't' || c == '\\')
            {
                b_ex = false;
                eNextState = STRING2_STATE;
            }
        }
        break;
        case STRINGU_STATE: // "\u
        {
            if (c == '{')
            {
                stringCounter = 0;
                b_ex = false;
                eNextState = STRINGU2_STATE;
            }
            else
            {
                //TODO ERROR
                return false;
            }
        }
        break;
        case STRINGU2_STATE: // "\u{
        {
            if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
            {
                stringCounter += 1;
                b_ex = false;
                eNextState = STRINGU3_STATE;
            }
            else
            {
                //TODO ERROR
                return false;
            }
        }
        break;
        case STRINGU3_STATE: // "\u{NIECO
        {
            printf("character is: %c\n",c);
            if(stringCounter <= 8)
            {
                if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
                {
                    stringCounter += 1;
                    b_ex = false;
                    eNextState = STRINGU3_STATE;
                }
                else if (c == '}')
                {
                    b_ex = false;
                    eNextState = STRING2_STATE;
                }
                else
                {
                    // TODO error
                    // printf("neplatne cislo chalan\n");
                    return false;
                }
            }
            else
            {
                //TODO ERROR
                // printf("vela cisel chalan\n");
                return false;
            }
        }
        break;
        case STRINGMULTI_STATE: // ""
        {
            if (c == '"')
            {
                b_ex = false;
                eNextState = STRINGMULTI2_STATE;
            }
            else
            {
                //TODO ERROR
                return false;
            }
        }
        break;
        case STRINGMULTI2_STATE: // """
        {
            if (c == 10)
            {
                b_ex = false;
                eNextState = STRINGMULTI3_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRINGMULTI4_STATE;
            }
        }
        break;
        case STRINGMULTI3_STATE: // """ \n
        {
           if (c == 10)
            {
                b_ex = false;
                eNextState = STRINGMULTI3_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRINGMULTI4_STATE;
            }
        }
        break;
        case STRINGMULTI4_STATE: // """ something
        {
           if (c == 10)
            {
                b_ex = false;
                eNextState = STRINGMULTI5_STATE;
            }
            else if (c == '"')
            {
                b_ex = false;
                eNextState = STRINGMULTI6_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRINGMULTI4_STATE;
            }
        }
        break;
        case STRINGMULTI5_STATE: // """ something \n
        {
           if (c == 10)
            {
                b_ex = false;
                eNextState = STRINGMULTI5_STATE;
            }
            else if (c == '"')
            {
                b_ex = false;
                eNextState = STRINGMULTI6_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRINGMULTI4_STATE;
            }
        }
        break;
        case STRINGMULTI6_STATE: // """ something "
        {
            if (c == '"')
            {
                b_ex = false;
                eNextState = STRINGMULTI7_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRINGMULTI4_STATE;
            }
            else
            {
                // TODO ERROR
                return false;
            }
        }
        break;
        case STRINGMULTI7_STATE: // """ something ""
        {
            if (c == '"')
            {
                b_ex = false;
                eNextState = STRINGMULTI8_STATE;
            }
            else if (c > 31)
            {
                b_ex = false;
                eNextState = STRINGMULTI4_STATE;
            }
            else
            {
                // TODO ERROR
                return false;
            }
        }
        break;
        case STRINGMULTI8_STATE: // """ something """
        {
            *type = stringT;
            condition = false;
            b_ex = true;
            eNextState = START_STATE;
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
        else
        {
            //debug print
            dynstr_print(buffer);
            if (compareWithTable)
            {
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
                else if ((dynstr_cmp(buffer, "Double")) || (dynstr_cmp(buffer, "Double?")))
                {
                    *type = vartypeT;
                }
                else if ((dynstr_cmp(buffer, "Int")) || (dynstr_cmp(buffer, "Int?")))
                {
                    *type = vartypeT;
                }
                else if ((dynstr_cmp(buffer, "String")) || (dynstr_cmp(buffer, "String?")))
                {
                    *type = vartypeT;
                }
                else if (dynstr_cmp(buffer, "nil"))
                {
                    *type = nilT;
                }
                else if (dynstr_cmp(buffer, "return"))
                {
                    *type = returnT;
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