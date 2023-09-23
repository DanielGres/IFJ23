CC = gcc
CFLAGS = -Wall -g

lexer: lexer.c lexer.h
	$(CC) $(CFLAGS) -o lexer lexer.c

clean:
	rm -f lexer