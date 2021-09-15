vpath %.c src
vpath %.o bin
project1: main.o symbols.o list.o
	gcc -Wall -o project1 $^

main.o: main.c
	gcc -Wall -O0 -c $^ -o $@

symbols.o: symbols.c
	gcc -Wall -O0 -c $^ -o $@

list.o: list.c
	gcc -Wall -O0 -c $^ -o $@


clean:
	rm *.o -f
	touch *.c
	rm project1 -f
