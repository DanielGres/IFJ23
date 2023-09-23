#include <stdio.h>
#include "lexer.h"

char readCharacter(FILE *file) {
    char character = fgetc(file);
    return character;
}

int main(int argc, char *argv[]) {
    
    const char *strings[] = {
        "Double",
        "else",
        "func",
        "if",
        "Int",
        "let",
        "nil",
        "return",
        "String",
        "var",
        "while"
    };

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    SM_STATE eNextState = START_STATE;

    char c;
    
    // Read characters until EOF (End of File) is encountered
    while ((c = readCharacter(file)) != EOF) {
        printf("%c\n",c);
        switch(eNextState){
            case START_STATE:
            {
                if(c == '_' || (c >= 'a' && c <= 'z')){
                    eNextState = ID_STATE;
                }
                if(c == '('){
                    eNextState = LEFTBRACKET_STATE;
                }
                if(c == ')'){
                    eNextState = RIGHTBRACKET_STATE;
                }
                if(c >= '0' && c <= '9'){
                    eNextState = NUMBER_STATE;
                }
            }
            case ID_STATE:
            {
                if(c == '_' || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
                    eNextState = ID_STATE;
                }
                else{
                    eNextState = START_STATE;
                }

            }
            case LEFTBRACKET_STATE:
            {
                eNextState = START_STATE;
            }
            case RIGHTBRACKET_STATE:
            {
                eNextState = START_STATE;
            }
        }
    }

    

    fclose(file);
    return 0;
}