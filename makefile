all: UsoDisco

UsoDisco: main.o
	gcc main.o -o UsoDisco

main.o: main.c
	gcc -c main.c

clean:
	rm -f *.o *.gch main || true
	rm UsoDisco || true