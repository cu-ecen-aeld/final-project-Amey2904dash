
CC = gcc 
CFLAGS = -Wall -Werror
LDFLAGS = -lrt -lgpiod -pthread
OBJFILES = wiringPi.o lcd.o
TARGET = lcd
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
