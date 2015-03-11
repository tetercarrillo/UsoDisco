all: UsoDisco

UsoDisco: directorio.o main.o cola.o
	gcc directorio.o cola.o main.o -o UsoDisco

main.o: main.c
	gcc -c main.c

directorio.o: directorio.c directorio.h cola.h
	gcc -c directorio.c

cola.o: cola.c cola.h
	gcc -c cola.c

clean:
	rm -f *.o *.gch main || true
	rm UsoDisco || true