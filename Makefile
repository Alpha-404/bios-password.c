CC = gcc
CFLAGS = -O2 -Wall -DENABLE_SONY -DENABLE_HPMINI -DENABLE_PHOENIX -DENABLE_HPAMI# add -O2 for optim
LDFLAGS = -lm
SRCS = main.c utils/solver.c utils/utils.c utils/crc32.c keygens/sony.c keygens/hpmini.c keygens/phoenix.c keygens/hpami.c
OBJS = $(SRCS:.c=.o)
TARGET = keygen

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o utils/*.o keygens/*.o
