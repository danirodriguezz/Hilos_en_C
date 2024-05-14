producto: funciones.o main.o
		gcc -o producto funciones.o main.o -lpthread
main.o: main.c
		gcc -c main.c
funciones.o: funciones.c
		gcc -c funciones.c
clean:
		rm *.o