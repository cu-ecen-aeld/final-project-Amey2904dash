CROSS_COMPILE=
CC = $(CROSS_COMPILE)gcc 
CFLAGS = -Wall -Werror
LDFLAGS = -lrt -pthread
OBJFILES = wiringPi.o lcd.o socket_client.o
TARGET = socket_client
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -rf $(OBJFILES) $(TARGET) *~
