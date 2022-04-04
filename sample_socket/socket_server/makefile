CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: socket_server
default: socket_server


server.o: server.c
	$(CC) $(CFLAGS) -c socket_server.c

server : socket_server.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) socket_server.o -o socket_server

clean :
	rm -f socket_server *.o
