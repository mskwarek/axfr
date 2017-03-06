all:
	gcc -c dns.c -o dns.o -ldns
	g++ -c main.cpp -o main.o -I.
	g++ main.o dns.o
clean:
	rm -rf *.o
