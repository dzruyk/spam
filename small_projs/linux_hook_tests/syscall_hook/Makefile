CC=gcc
CFLAGS=-Wall

TESTOBJS=tested.o
TESTER=test_write
RUYKLIBOBJS=hook.o
RUYKLIB=libruyk.so


all: $(TESTER) $(RUYKLIB)

$(TESTER): $(TESTOBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(RUYKLIB): $(RUYKLIBOBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^
clean:
	rm -f *.o  *~ $(RUYKLIB) $(TESTER)
