CC = gcc
CFLAGS = -Wall -g

# Get a list of all .c files in the current directory
SRC_FILES = $(wildcard *.c)

# Generate a list of corresponding .o files from .c files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Define the executable name
EXECUTABLE = ifjcomp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES)
