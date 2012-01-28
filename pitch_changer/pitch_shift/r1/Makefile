CC = gcc

CFLAGS = -Wall
LIBS = -lasound

PITH_SHIFT_OBJS = smbPitchShift.o convert.o main.o
PITH_SHIFT_TARGET = pitch_shifter

all: $(PITH_SHIFT_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(PITH_SHIFT_TARGET): $(PITH_SHIFT_OBJS)
	$(CC) $(CFLAGS) -o $@  $(PITH_SHIFT_OBJS) -lm

clean:
	rm -rf *.o *~
