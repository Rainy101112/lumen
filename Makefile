CC			:= clang
C_FLAGS		:= -Wall -Wextra -O2 -g3 -I include

all: build

build:
	$(CC) -c $(C_FLAGS) src/main.c -o main.o
	$(CC) -c $(C_FLAGS) src/lexer.c -o lexer.o
	$(CC) -c $(C_FLAGS) src/parser.c -o parser.o
	$(CC) main.o lexer.o parser.o $(C_FLAGS) -o lumen 

.PHONY: clean format

clean:
	rm *.o
	rm lumen

format:
	clang-format