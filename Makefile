CC = gcc
CFLAGS = -Wall -g

# Get a list of all .c files in the current directory
SRC_FILES = $(wildcard src/*.c)

# Generate a list of corresponding .o files from .c files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Dependecies
DEPS = include/$(wildcard *.h)
# Define the executable name
EXECUTABLE = ifjcomp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES) $(DEPS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES)
