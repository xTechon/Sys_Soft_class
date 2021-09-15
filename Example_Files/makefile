project1:	main.o symbols.o  directives.o headers.h
	gcc -o project1 -Wall  -O0 main.o symbols.o directives.o

main.o:	main.c headers.h 
	gcc -c -Wall  -O0 main.c

symbols.o:	symbols.c headers.h
	gcc -c -Wall  -O0 symbols.c

directives.o:	directives.c headers.h
	gcc -c -Wall  -O0 directives.c


clean:	
	rm *.o -f
	touch *.c
	rm project1 -f
