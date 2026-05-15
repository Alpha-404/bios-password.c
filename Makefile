CC = gcc
CFLAGS = -O2 -Wall -DENABLE_SONY -DENABLE_HPMINI -DENABLE_PHOENIX -DENABLE_HPAMI -DENABLE_INSYDE -DENABLE_ASUS -DENABLE_DELL # add -O2 for optim, add -O0 -g for debug
LDFLAGS = -lm
SRCS = main.c utils/solver.c utils/utils.c utils/crc32.c utils/crc64.c utils/sha256.c utils/aes128.c keygens/sony.c keygens/hpmini.c keygens/phoenix.c keygens/hpami.c keygens/insyde.c keygens/asus.c keygens/dell.c
OBJS = $(SRCS:.c=.o)
TARGET = keygen

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o utils/*.o keygens/*.o
