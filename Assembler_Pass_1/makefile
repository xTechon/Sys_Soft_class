vpath %.c src
vpath %.o bin
project1: main.o symbols.o
	gcc -o project1 $^

main.o: main.c
	gcc -O0 -c $^ -o $@

symbols.o: symbols.c
	gcc -O0 -c $^ -o $@


clean:
	rm *.o -f
	touch *.c
	rm project1 -f
