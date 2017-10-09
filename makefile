TARGET = p1

p1: hw1.o
	hw1

hw1.o:
	gcc concurrency.c -o hw1 -pthread -lpthread -std=c99


clean:
	rm -f *.o hw1 *~ *.pdf *.log *.out *.blg

all:
	
