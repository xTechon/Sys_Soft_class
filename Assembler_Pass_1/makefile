vpath %.c src
vpath %.o bin
CC = gcc -Wall -O0
project1: main.o symbols.o list.o tree.o opcodes.o hash.o src/headers.h
	gcc -Wall -o project1 $^

main.o: main.c src/headers.h
	$(CC) -c $^

symbols.o: symbols.c src/headers.h
	$(CC) -c $^

list.o: list.c src/headers.h
	$(CC) -c $^

tree.o: tree.c src/headers.h
	$(CC) -c $^

opcodes.o: opcodes.c src/headers.h
	$(CC) -c $^

hash.o: hash.c src/headers.h
	$(CC) -c $^

clean:
	rm *.o bin/*.o -f
	touch src/*.c
	rm project1 -f
	rm *.sic
	rm grade
