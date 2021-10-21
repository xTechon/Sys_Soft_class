vpath %.c src
vpath %.o bin
CC = gcc -Wall -O0 -c -g
project1: main.o symbols.o list.o tree.o opcodes.o hash.o src/headers.h
	gcc -Wall -g -o project1 $^

main.o: main.c src/headers.h
	$(CC) $^

symbols.o: symbols.c src/headers.h
	$(CC) $^

list.o: list.c src/headers.h
	$(CC) $^

tree.o: tree.c src/headers.h
	$(CC) $^

opcodes.o: opcodes.c src/headers.h
	$(CC) $^

hash.o: hash.c src/headers.h
	$(CC) $^

clean:
	rm *.o bin/*.o -f
	touch src/*.c
	rm project1 -f
	rm *.sic
	rm grade
