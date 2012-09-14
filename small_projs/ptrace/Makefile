CC=gcc
CFLAGS= -Wall -g

PATIENT=patient
PATIENT_OBJS=patient.o

TARGET=my_debugger
OBJS=main.o

SIMPLE_TEST_OBJS=test.o
SIMPLE_TEST=_test

all: $(PATIENT) $(TARGET) $(SIMPLE_TEST)

%.o: %.c
	$(CC) $(CFLAGS) -c $^
%.o: %.S
	$(CC) $(CFLAGS) -c $^

$(PATIENT): $(PATIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SIMPLE_TEST): $(SIMPLE_TEST_OBJS)
	ld -o $@ $^

clean:
	rm -rf *~ *.o $(TARGET) $(PATIENT) $(SIMPLE_TEST)
