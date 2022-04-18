INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g -Wall -Werror  $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= multi_thread_safe.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	multi_thread_safe

clean:
	-rm -f *.o *.d
	-rm -f multi_thread_safe

multi_thread_safe: multi_thread_safe.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
