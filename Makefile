FLAGS = -Wextra -Wpedantic -Wall -g -std=c11

OBJ = main.o ast.o parser.o

TARGET = interpreter

$(TARGET): $(OBJ)
	gcc $(FLAGS) $(OBJ) -o $(TARGET)

main.o: main.c 
	gcc $(FLAGS) -c main.c 

lexer.o: lexer.c
	gcc $(FLAGS) -c lexer.c

ast.o: ast.c
	gcc $(FLAGS) -c ast.c

parser.o: parser.c
	gcc $(FLAGS) -c parser.c
