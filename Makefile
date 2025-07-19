CC = gcc
CFLAGS = -Wall -O2
SRCS = main.c keygens/sony.c
OBJS = $(SRCS:.c=.o)
TARGET = keygen

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.o
