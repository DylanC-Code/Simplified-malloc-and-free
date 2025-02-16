all: main.exe
	./main.exe

main.exe: main.o heap.o heap.h
	gcc -o main.exe main.o heap.o -lm

main.o: main.c
	gcc -c main.c

heap.o: heap.c
	gcc -c heap.c

clean:
	rm *.o *.exe