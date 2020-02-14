
all: mandel mandelmovie

mandel: mandel.o bitmap.o
	gcc mandel.o bitmap.o -o mandel -lpthread -lm

mandel.o: mandel.c
	gcc -Wall -g -c mandel.c -o mandel.o 

bitmap.o: bitmap.c
	gcc -Wall -g -c bitmap.c -o bitmap.o 

mandelmovie: mandelmovie.c 
	gcc -Wall mandelmovie.c -o mandelmovie -lm



clean:
	rm -f mandel.o bitmap.o mandel
	rm -f mandelmovie
