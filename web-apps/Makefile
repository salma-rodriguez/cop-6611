all: test.o client.o server.o
	gcc test.o -lpthread -o test
	gcc client.o -o client
	gcc server.o -lpthread -o server

server.o: server.c
	gcc -c server.c

test.o: test.c
	gcc -c test.c

client.o: client.c
	gcc -c client.c

clean:
	rm -f test client server *.o
