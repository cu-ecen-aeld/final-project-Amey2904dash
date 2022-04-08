
CC=gcc


CFLAGS= -g -Wall -Werror


all: gpiodriver
default: gpiodriver


gpiodriver.o: gpiodriver.c
	$(CC) $(CFLAGS) -c gpiodriver.c

gpiodriver : gpiodriver.o
	$(CROSS_COMPILE)$(CC) $(CFLAGS) gpiodriver.o -o gpiodriver

clean :
	rm -f gpiodriver *.o
