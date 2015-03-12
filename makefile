all: UsoDisco

UsoDisco: directorio.o main.o
	gcc directorio.o main.o -o UsoDisco

main.o: main.c directorio.h
	gcc -c main.c

directorio.o: directorio.c directorio.h
	gcc -c directorio.c

clean:
	rm -f *.o *.gch main || true
