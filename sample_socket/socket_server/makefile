


CC=aarch64-none-linux-gnu-gcc


CFLAGS= -g -Wall -Werror


all: socket_server
default: socket_server


server.o: server.c
	$(CC) $(CFLAGS) -c socket_server.c

server : socket_server.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) socket_server.o -o socket_server

clean :
	rm -f socket_server *.o
