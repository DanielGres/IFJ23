#include <stdio.h>

int readCharacter(FILE *file) {
    int character = fgetc(file);
    return character;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    int character;
    
    // Read characters until EOF (End of File) is encountered
    while ((character = readCharacter(file)) != EOF) {
        printf("%c\n",character);
        //putchar(character);
    }

    fclose(file);
    return 0;
}