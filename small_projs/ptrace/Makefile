CC=gcc
CFLAGS= -Wall -g

PATIENT=patient
PATIENT_OBJS=patient.o

TARGET=my_debugger
OBJS=main.o


all: $(PATIENT) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

$(PATIENT): $(PATIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $>

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $>

clean:
	rm -rf *~
	rm *.o
	rm $(TARGET)
	rm $(PATIENT)
