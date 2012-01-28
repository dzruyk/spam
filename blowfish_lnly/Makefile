CC = gcc
FLAGS = -Wall

SRC= main.c blowfish.c
OBJS = main.o blowfish.o

TARGET = test_blowfish


all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -rf *.o
	rm -rf *~
	rm -rf $(TARGET)

