CC=gcc -g
CFLAGS=-c -Wall

all: webserver

webserver: server.o routes.o
	$(CC) server.o routes.o -o webserver
	
server.o: server.c server.h
	$(CC) $(CFLAGS) server.c -o server.o

routes.o: routes.c routes.h
	$(CC) $(CFLAGS) routes.c -o routes.o

clean:
	rm -rf *.o