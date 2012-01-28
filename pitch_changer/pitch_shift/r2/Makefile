CC = gcc

CFLAGS = -Wall
LIBS = -lasound

EXAMPLE3_OBJS = example3.o
EXAMPLE4_OBJS = example4.o
PITH_SHIFT_OBJS = smbPitchShift.o main.o

EXAMPLE3_TARGET = sound_play
EXAMPLE4_TARGET = sound_rec
PITH_SHIFT_TARGET = pitch_shifter

all: $(EXAMPLE3_TARGET) $(EXAMPLE4_TARGET) $(PITH_SHIFT_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


$(EXAMPLE3_TARGET): $(EXAMPLE3_OBJS)
	$(CC) $(CFLAGS) -o $@ $(EXAMPLE3_OBJS) $(LIBS)

$(EXAMPLE4_TARGET): $(EXAMPLE4_OBJS)
	$(CC) $(CFLAGS) -o $@ $(EXAMPLE4_OBJS) $(LIBS)

$(PITH_SHIFT_TARGET): $(PITH_SHIFT_OBJS)
	$(CC) $(CFLAGS) -o $@  $(PITH_SHIFT_OBJS) -lm

clean:
	rm -rf *.o *~


