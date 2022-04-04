CROSS_COMPILE=

ifeq ($(CC),)
	CC=$(CROSS_COMPILE)gcc
endif

ifeq ($(CFLAGS),)
	CFLAGS= -g -Wall -Werror
endif

all: socket_client
default: socket_client


socket_client.o: socket_client.c
	$(CC) $(CFLAGS) -c socket_client.c

socket_client : socket_client.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) socket_client.o -o socket_client

clean :
	rm -f socket_client *.o
