CC=gcc
CFLAGS=-Wall -ggdb
OBJS=search.o common.o

VALID_O=validation.o
VALIDATION=validation_test

PERF_O=performance.o
PERFORMANCE=performance_test

all: $(VALIDATION) $(PERFORMANCE)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

$(VALIDATION): $(OBJS) $(VALID_O)
	$(CC) $(CFLAGS) -o $@ $>

$(PERFORMANCE): $(OBJS) $(PERF_O)
	$(CC) $(CFLAGS) -o $@ $>

clean:
	rm -rf *~ *.o
	rm $(VALIDATION)

