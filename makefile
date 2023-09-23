CC = gcc
CFLAGS = -Wall -g

lexer: lexer.c
	$(CC) $(CFLAGS) -o lexer lexer.c

clean:
	rm -f lexer