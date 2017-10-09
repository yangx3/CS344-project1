TARGET = p1

p1: hw1.o
	hw1

hw1.o: all
	gcc concurrency.c -o hw1 -pthread -lpthread -std=c99


clean:
	rm -f *.o hw1 *~ *.pdf *.log *.out *.blg

all:
	latex p1.tex
	dvips -R -Poutline -t letter p1.dvi -o p1.ps
	ps2pdf p1.ps
	rm p1.aux
	rm p1.dvi
	rm p1.log
	rm p1.ps
	rm p1.out
	
