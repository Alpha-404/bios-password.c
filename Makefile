CC = gcc
CFLAGS = -Wall -O2
SRCS = main.c utils/solver.c utils/utils.c keygens/sony.c keygens/hpmini.c
OBJS = $(SRCS:.c=.o)
TARGET = keygen

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.o utils/*.o keygens/*.o
