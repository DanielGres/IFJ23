// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#include "lexer.h"

bool stop = false;
bool b_ex = false;
char c = 'a';
int stringCounter = 0;

bool Get_Token(token **T)  // TODO podla mna by toto malo byt v syntaxi
{
    if (!stop) {
        dyn_string buffer;
        token_type tdt;
        Token_init(T);
        dynstr_init(&buffer);
        if (lexer(&buffer, &tdt)) {
            Token_set(T, &buffer, tdt);
            return true;
        } else {
            Token_set(T, &buffer, eofT);
            return false;
        }
    }
    return false;
}

// Function to convert a single hexadecimal character to an integer
int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;  // Invalid character
}

// Function to convert a hexadecimal string to its decimal representation
int hexToDecimal(dyn_string *hex) {
    int decimal = 0;
    for (int i = 0; i < hex->len; i++) {
        int hexValue = hexCharToInt(hex->s[i]);
        if (hexValue == -1) {
            // Invalid hex character
            return -1;
        }
        decimal = decimal * 16 + hexValue;
    }
    return decimal;
}

void replaceUnicodeSequences(dyn_string *inputString) {
    dyn_string resultString;
    dynstr_init(&resultString);

    int i = 0;
    while (i < inputString->len) {
        if (inputString->s[i] == '\\' && inputString->s[i + 1] == 'u' && inputString->s[i + 2] == '{') {
            int start = i + 3;
            while (i < inputString->len && inputString->s[i] != '}') {
                i++;
            }
            int end = i;
            i++;  // Skip the closing '}'

            // Extract the hexadecimal sequence (excluding the closing curly bracket)
            dyn_string hexSequence;
            dynstr_init(&hexSequence);
            for (int j = start; j < end; j++) {
                if (inputString->s[j] != '}') {
                    dynstr_add(&hexSequence, inputString->s[j]);
                }
            }

            // Convert the hexadecimal sequence to decimal
            int decimalValue = hexToDecimal(&hexSequence);
            dynstr_destr(&hexSequence);

            // Append the corresponding ASCII character to the result string
            char asciiChar = (char)decimalValue;
            dynstr_add(&resultString, asciiChar);
        } else if (inputString->s[i] == '\t') {
            // Replace tab with the corresponding escape sequence
            dynstr_addstr(&resultString, "\\t");
            i++;
        } else if (inputString->s[i] == '\n') {
            // Replace newline with the corresponding escape sequence
            dynstr_addstr(&resultString, "\\n");
            i++;
        } else if (inputString->s[i] == '\r') {
            // Replace carriage return with the corresponding escape sequence
            dynstr_addstr(&resultString, "\\r");
            i++;
        } else if (inputString->s[i] == ' ') {
            // Replace space with the corresponding ASCII value (32)
            dynstr_addstr(&resultString, "\\032");
            i++;
        } else if (inputString->s[i] == '\\' && i + 1 < inputString->len && inputString->s[i + 1] == 'n') {
            // Replace the actual characters "\n" with the escape sequence "\010"
            dynstr_addstr(&resultString, "\\010");
            i += 2;
        } else if (inputString->s[i] == '\\' && i + 1 < inputString->len && inputString->s[i + 1] == 't') {
            // Replace the actual characters "\t" with the escape sequence "\009"
            dynstr_addstr(&resultString, "\\009");
            i += 2;
        } else if (inputString->s[i] == '\\' && i + 1 < inputString->len && inputString->s[i + 1] == 'r') {
            // Replace the actual characters "\r" with the escape sequence "\013"
            dynstr_addstr(&resultString, "\\013");
            i += 2;
        } else {
            dynstr_add(&resultString, inputString->s[i]);
            i++;
        }
    }
    dynstr_copy(inputString, &resultString);
    dynstr_destr(&resultString);
}

bool lexer(dyn_string *buffer, token_type *type) {
    SM_STATE eNextState = START_STATE;
    bool condition = true;
    bool compareWithTable = false;
    bool ignore;
    dyn_string errMSG;
    dynstr_init(&errMSG);

    while (condition) {
        if (!b_ex) {
            c = fgetc(stdin);
        }
        ignore = false;
        switch (eNextState) {
            case START_STATE: {
                if (c == '_') {
                    b_ex = false;
                    eNextState = ID_STATE_0;
                } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                    b_ex = false;
                    eNextState = ID_STATE;
                } else if (c == '(') {
                    b_ex = false;
                    eNextState = LEFTBRACKET_STATE;
                } else if (c == ')') {
                    b_ex = false;
                    eNextState = RIGHTBRACKET_STATE;
                } else if (c == '{') {
                    b_ex = false;
                    eNextState = LEFTCURLYBRACKET_STATE;
                } else if (c == '}') {
                    b_ex = false;
                    eNextState = RIGHTCURLYBRACKET_STATE;
                } else if (c >= '0' && c <= '9') {
                    b_ex = false;
                    eNextState = NUMBER_STATE;
                } else if (c == '"') {
                    b_ex = false;
                    ignore = true;
                    eNextState = STRING_STATE;
                } else if (c == ':') {
                    b_ex = false;
                    eNextState = COLON_STATE;
                } else if (c == '=') {
                    b_ex = false;
                    eNextState = EQUAL_STATE;
                } else if (c == '+') {
                    b_ex = false;
                    eNextState = PLUS_STATE;
                } else if (c == '-') {
                    b_ex = false;
                    eNextState = MINUS_STATE;
                } else if (c == '!') {
                    b_ex = false;
                    eNextState = EXCLAMATION_STATE;
                } else if (c == '*') {
                    b_ex = false;
                    eNextState = MULTIP_STATE;
                } else if (c == '/') {
                    b_ex = false;
                    eNextState = SLASH_STATE;
                } else if (c == '>') {
                    b_ex = false;
                    eNextState = GREATER_STATE;
                } else if (c == '<') {
                    b_ex = false;
                    eNextState = LESS_STATE;
                } else if (c == '?') {
                    b_ex = false;
                    eNextState = QUESTION_STATE;
                } else if (c == ',') {
                    b_ex = false;
                    eNextState = COMMA_STATE;
                } else if (c == ';') {
                    b_ex = false;
                    eNextState = SEMICOL_STATE;
                } else if (c == 10) {
                    b_ex = false;
                    eNextState = NEWLINE_STATE;
                } else if (c == 32) {  // Exitus
                    b_ex = false;
                    eNextState = START_STATE;
                } else if (c == EOF) {
                    return false;
                } else {
                    b_ex = false;
                    condition = false;
                    dynstr_addstr(&errMSG, "Invalid input character!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case NEWLINE_STATE:  // \n
            {
                if (c == 10 || c == 9 || c == 13 || c == 32) {
                    condition = true;
                    b_ex = false;
                    eNextState = NEWLINE_STATE;
                } else {
                    condition = false;
                    b_ex = true;
                    *type = newlineT;
                    eNextState = START_STATE;
                }
            } break;
            case SEMICOL_STATE:  // ;
            {
                *type = semicolonT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            } break;
            case COMMA_STATE:  // ,
            {
                *type = commaT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            } break;
            case QUESTION_STATE:  // ?
            {
                if (c == '?') {
                    b_ex = false;
                    eNextState = QUESTION2_STATE;
                } else {
                    b_ex = false;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected a question mark (?)!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case QUESTION2_STATE:  // ??
            {
                *type = operatorT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            } break;
            case GREATER_STATE:  // >
            {
                if (c == '=') {
                    b_ex = false;
                    eNextState = GREATEREQUAL_STATE;
                } else {
                    *type = operatorT;
                    b_ex = true;
                    condition = false;
                    eNextState = START_STATE;
                }
            } break;
            case LESS_STATE:  // <
            {
                if (c == '=') {
                    b_ex = false;
                    eNextState = LESSEQUAL_STATE;
                } else {
                    *type = operatorT;
                    b_ex = true;
                    condition = false;
                    eNextState = START_STATE;
                }
            } break;
            case GREATEREQUAL_STATE:  // >=
            {
                *type = operatorT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            } break;
            case LESSEQUAL_STATE:  // <=
            {
                *type = operatorT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            } break;
            case SLASH_STATE:  // /
            {
                if (c == '/') {
                    b_ex = false;
                    eNextState = LINECOMM_STATE;
                } else if (c == '*') {
                    b_ex = false;
                    eNextState = BLOCKCOMM_STATE;
                } else {
                    *type = operatorT;
                    b_ex = true;
                    condition = false;
                    eNextState = START_STATE;
                }
            } break;
            case LINECOMM_STATE:  // //
            {
                if (c != '\n') {
                    b_ex = false;
                    eNextState = LINECOMM_STATE;
                } else if (c == '\n') {
                    b_ex = false;
                    eNextState = LINECOMM2_STATE;
                }
            } break;
            case LINECOMM2_STATE:  // //comment
            {
                *type = linecommentT;
                b_ex = true;
                condition = false;
                eNextState = START_STATE;
            } break;
            case BLOCKCOMM_STATE:  // /* comment
            {
                if (c != '*') {
                    b_ex = false;
                    eNextState = BLOCKCOMM_STATE;
                } else if (c == '*') {
                    b_ex = false;
                    eNextState = BLOCKCOMM2_STATE;
                }
            } break;
            case BLOCKCOMM2_STATE:  // /* comment *
            {
                if (c == '/') {
                    b_ex = false;
                    eNextState = BLOCKCOMM3_STATE;
                } else if (c == '*') {
                    b_ex = false;
                    eNextState = BLOCKCOMM2_STATE;
                } else if (c != '*') {
                    b_ex = false;
                    eNextState = BLOCKCOMM_STATE;
                }
            } break;
            case BLOCKCOMM3_STATE:  // /* comment */
            {
                *type = blockcommentT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case MULTIP_STATE:  // *
            {
                *type = operatorT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case MINUS_STATE:  // -
            {
                if (c == '>') {
                    b_ex = false;
                    eNextState = ARROW_STATE;
                } else {
                    *type = operatorT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case ARROW_STATE:  // ->
            {
                *type = arrowT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case PLUS_STATE:  // +
            {
                *type = operatorT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case COLON_STATE:  // :
            {
                *type = colonT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case EQUAL_STATE:  // =
            {
                if (c == '=') {
                    b_ex = false;
                    eNextState = COMPARE_STATE;
                } else {
                    *type = equalT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case COMPARE_STATE:  // ==
            {
                *type = operatorT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case EXCLAMATION_STATE:  // !
            {
                if (c == '=') {
                    b_ex = false;
                    eNextState = NOTEQUAL_STATE;
                } else {
                    *type = operatorT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case NOTEQUAL_STATE:  // !=
            {
                *type = operatorT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case ID_STATE_0:  // _
            {
                if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
                    b_ex = false;
                    eNextState = ID2_STATE;
                } else {
                    *type = underscoreT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case ID_STATE:  // _
            {
                if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
                    b_ex = false;
                    eNextState = ID2_STATE;
                } else {
                    *type = varidT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case ID2_STATE:  // __
            {
                if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
                    b_ex = false;
                    eNextState = ID2_STATE;
                } else if (c == '?') {
                    b_ex = false;
                    eNextState = IDTYPE_STATE;
                } else {
                    *type = varidT;
                    condition = false;
                    compareWithTable = true;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case IDTYPE_STATE: {
                *type = vartypeQT;
                condition = false;
                compareWithTable = true;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case LEFTBRACKET_STATE: {
                *type = LbracketT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case RIGHTBRACKET_STATE: {
                *type = RbracketT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case LEFTCURLYBRACKET_STATE: {
                *type = LCbracketT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case RIGHTCURLYBRACKET_STATE: {
                *type = RCbracketT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case NUMBER_STATE:  // [0-9]
            {
                if (c >= '0' && c <= '9') {
                    b_ex = false;
                    eNextState = NUMBER_STATE;
                } else if (c == '.') {
                    b_ex = false;
                    eNextState = DECNUMBER1_STATE;
                } else if (c == 'e' || c == 'E') {
                    b_ex = false;
                    eNextState = EXPNUMBER1_STATE;
                } else {
                    *type = intnumT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case DECNUMBER1_STATE:  // [0-9].
            {
                if (c >= '0' && c <= '9') {
                    b_ex = false;
                    eNextState = DECNUMBER2_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected a number!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case DECNUMBER2_STATE:  // [0-9].[0-9]
            {
                if (c >= '0' && c <= '9') {
                    b_ex = false;
                    eNextState = DECNUMBER2_STATE;
                } else if (c == 'e' || c == 'E') {
                    eNextState = EXPNUMBER1_STATE;
                } else {
                    *type = doublenumT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case EXPNUMBER1_STATE:  // [0-9]eE or [0-9].[0-9]eE
            {
                if (c >= '0' && c <= '9') {
                    b_ex = false;
                    eNextState = EXPNUMBER3_STATE;
                } else if (c == '+' || c == '-') {
                    b_ex = false;
                    eNextState = EXPNUMBER2_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected a number or an operator (+ or -)!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case EXPNUMBER2_STATE:  // [0-9]eE[+-] or [0-9].[0-9]eE[+-]
            {
                if (c >= '0' && c <= '9') {
                    b_ex = false;
                    eNextState = EXPNUMBER3_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected a number!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case EXPNUMBER3_STATE:  // [0-9]eE[0-9]
            {
                if (c >= '0' && c <= '9') {
                    b_ex = false;
                    eNextState = EXPNUMBER3_STATE;
                } else {
                    *type = doublenumT;
                    condition = false;
                    b_ex = true;
                    eNextState = START_STATE;
                }
            } break;
            case STRING_STATE:  // "
            {
                if (c == '\\') {
                    b_ex = false;
                    eNextState = STRINGESCAPE_STATE;
                } else if (c == '"') {
                    b_ex = false;
                    ignore = true;
                    eNextState = STRINGMULTI_STATE;  // TODO ignore multi string quotes for now
                } else if (c > 31) {
                    b_ex = false;
                    eNextState = STRING2_STATE;
                }
            } break;
            case STRING2_STATE:  // "something
            {
                if (c == '"') {
                    b_ex = false;
                    ignore = true;
                    eNextState = STRING3_STATE;
                } else if (c == '\\') {
                    b_ex = false;
                    eNextState = STRINGESCAPE_STATE;
                } else if (c > 31) {
                    b_ex = false;
                    eNextState = STRING2_STATE;
                }
            } break;
            case STRING3_STATE: {
                replaceUnicodeSequences(buffer);
                *type = stringT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;
            case STRINGESCAPE_STATE:  // "backslash(\)
            {
                if (c == 'u') {
                    b_ex = false;
                    eNextState = STRINGU_STATE;
                } else if (c == '"' || c == 'r' || c == 'n' || c == 't' || c == '\\') {
                    b_ex = false;
                    eNextState = STRING2_STATE;
                }
            } break;
            case STRINGU_STATE:  // "\u
            {
                if (c == '{') {
                    stringCounter = 0;
                    b_ex = false;
                    eNextState = STRINGU2_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected '{'!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGU2_STATE:  // "\u{
            {
                if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
                    stringCounter += 1;
                    b_ex = false;
                    eNextState = STRINGU3_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected [0-9] or [Aa-Ff]!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGU3_STATE:  // "\u{NIECO
            {
                if (stringCounter <= 8) {
                    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
                        stringCounter += 1;
                        b_ex = false;
                        eNextState = STRINGU3_STATE;
                    } else if (c == '}') {
                        b_ex = false;
                        eNextState = STRING2_STATE;
                    } else {
                        b_ex = true;
                        condition = false;
                        dynstr_addstr(&errMSG, "Expected a number or an operator (+ or -)!\n");
                        errPrint(&errMSG, 1);
                        dynstr_destr(&errMSG);
                        exit(1);
                    }
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Too many number in escaped hex string!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGMULTI_STATE:  // ""
            {
                if (c == '"') {
                    b_ex = false;
                    eNextState = STRINGMULTI2_STATE;
                } else {
                    replaceUnicodeSequences(buffer);
                    *type = stringT;
                    condition = false;
                    b_ex = true;
                    ignore = true;
                    eNextState = START_STATE;
                }
            } break;
            case STRINGMULTI2_STATE:  // """
            {
                if (c == 10) {
                    b_ex = false;
                    eNextState = STRINGMULTI3_STATE;
                } else if (c == '"') {
                    b_ex = false;
                    eNextState = STRINGMULTI6_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected new line or double quotes!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGMULTI3_STATE:  // """ \n
            {
                if (c == 10) {
                    b_ex = false;
                    eNextState = STRINGMULTI3_STATE;
                } else if (c > 31) {
                    b_ex = false;
                    eNextState = STRINGMULTI4_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected newline or character!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGMULTI4_STATE:  // """ \n something
            {
                if (c == 10) {
                    b_ex = false;
                    eNextState = STRINGMULTI5_STATE;
                } else if (c > 31) {
                    b_ex = false;
                    eNextState = STRINGMULTI4_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected newline or character!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGMULTI5_STATE:  // """ \n something \n
            {
                if (c == 10) {
                    b_ex = false;
                    eNextState = STRINGMULTI5_STATE;
                } else if (c == '"') {
                    b_ex = false;
                    eNextState = STRINGMULTI6_STATE;
                } else if (c > 31) {
                    b_ex = false;
                    eNextState = STRINGMULTI4_STATE;
                }
            } break;
            case STRINGMULTI6_STATE:  // """ \n something \n "
            {
                if (c == 10) {
                    b_ex = false;
                    eNextState = STRINGMULTI5_STATE;
                } else if (c == '"') {
                    b_ex = false;
                    eNextState = STRINGMULTI7_STATE;
                } else if (c > 31) {
                    b_ex = false;
                    eNextState = STRINGMULTI4_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected doublequotes or a character!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGMULTI7_STATE:  // """ \n something \n  ""
            {
                if (c == '"') {
                    b_ex = false;
                    eNextState = STRINGMULTI8_STATE;
                } else if (c > 31) {
                    b_ex = false;
                    eNextState = STRINGMULTI4_STATE;
                } else {
                    b_ex = true;
                    condition = false;
                    dynstr_addstr(&errMSG, "Expected doublequotes or a character!\n");
                    errPrint(&errMSG, 1);
                    dynstr_destr(&errMSG);
                    exit(1);
                }
            } break;
            case STRINGMULTI8_STATE:  // """ \n something \n """
            {
                replaceUnicodeSequences(buffer);
                *type = stringT;
                condition = false;
                b_ex = true;
                eNextState = START_STATE;
            } break;

            default: {
                b_ex = true;
                condition = false;
                dynstr_addstr(&errMSG, "Invalid input character!\n");
                errPrint(&errMSG, 1);
                dynstr_destr(&errMSG);
                exit(1);
            }
        }
        if ((eNextState != START_STATE) && (!ignore)) {
            dynstr_add(buffer, c);
        } else {
            if (compareWithTable) {
                if (dynstr_cmp(buffer, "if")) {
                    *type = ifT;
                } else if (dynstr_cmp(buffer, "while")) {
                    *type = whileT;
                } else if (dynstr_cmp(buffer, "else")) {
                    *type = elseT;
                } else if (dynstr_cmp(buffer, "func")) {
                    *type = funcT;
                } else if (dynstr_cmp(buffer, "Double")) {
                    *type = vartypeT;
                } else if (dynstr_cmp(buffer, "Double?")) {
                    *type = vartypeQT;
                } else if (dynstr_cmp(buffer, "Int")) {
                    *type = vartypeT;
                } else if (dynstr_cmp(buffer, "Int?")) {
                    *type = vartypeQT;
                } else if (dynstr_cmp(buffer, "String")) {
                    *type = vartypeT;
                } else if (dynstr_cmp(buffer, "String?")) {
                    *type = vartypeQT;
                } else if (dynstr_cmp(buffer, "nil")) {
                    *type = nilT;
                } else if (dynstr_cmp(buffer, "return")) {
                    *type = returnT;
                } else if (dynstr_cmp(buffer, "var")) {
                    *type = varT;
                } else if (dynstr_cmp(buffer, "let")) {
                    *type = letT;
                }
            }
        }
    }
    return true;
}