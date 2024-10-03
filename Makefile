# RUN WITH THIS: mingw32-make

# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2

# All target
all: huffmanExecutable
# Target for building the huffmanExecutable from multiple object files
huffmanExecutable: huffman.o mainFile.o
	$(CC) $(CFLAGS) -o huffmanExecutable huffman.o mainFile.o

# Rule to compile huffman.c into an object file
huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

# Rule to compile mainFile.c into an object file
mainFile.o: mainFile.c huffman.h
	$(CC) $(CFLAGS) -c mainFile.c

# Clean target to remove object files and the executable
clean:
	rm -f *.o huffmanExecutable

# Run target to execute the program with specific arguments
run:
	./huffmanExecutable.exe -d