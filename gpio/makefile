
CC = $(CROSS_COMPILE)gcc

CFLAGS = -g -Wall -Werror

INCLUDES = -I/

all: lcd 


lcd.o: lcd.c
	$(CC) $(CFLAGS) -c lcd.c

lcd: lcd.o
	$(CC) $(CFLAGS) lcd.o -o lcd -lgpiod

clean:
	rm -f *.o
	rm -f lcd
