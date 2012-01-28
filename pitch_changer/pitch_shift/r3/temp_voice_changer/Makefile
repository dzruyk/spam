CC = gcc
CFLAGS = -Wall
LIBS = -lasound

OBJS = alsa_popygay.o fft.o pitch_shift.o
TARGET = popygay

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
