CC = gcc

CFLAGS = -Wall
LIBS = -lasound

AFREQ_OBJS = average_freq.o convert.o DFT.o fft.o
EXAMPLE4_OBJS = example4.o

AFREQ_TARGET = freq_test
EXAMPLE4_TARGET = sound_rec


all: $(EXAMPLE4_TARGET) $(AFREQ_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


$(EXAMPLE4_TARGET): $(EXAMPLE4_OBJS)
	$(CC) $(CFLAGS) -o $@ $(EXAMPLE4_OBJS) $(LIBS)

$(AFREQ_TARGET): $(AFREQ_OBJS)
	$(CC) $(CFLAGS) -o $@  $(AFREQ_OBJS) -lm

clean:
	rm -rf *.o *~
