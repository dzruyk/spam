CC=gcc
CFLAGS=-Wall -ggdb

LEX_OBJS = helper_funcs.o lex.o id_table.o hash.o primes.o keyword.o
LEX_MAIN = lex_test.o
LEX_TEST = lex_test
TRAVERSE_OBJS = stack.o syntax.o syn_tree.o traverse.o eval.o function.o 
TRAVERSE_MAIN = traverse_test.o
TRAVERSE_TEST = traverse_test

all: $(LEX_TEST) $(TRAVERSE_TEST)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

$(LEX_TEST): $(LEX_OBJS) $(LEX_MAIN)
	$(CC) $(CFLAGS) -o $(LEX_TEST) $(LEX_OBJS) $(LEX_MAIN)

$(TRAVERSE_TEST): $(LEX_OBJS) $(TRAVERSE_OBJS) $(TRAVERSE_MAIN)
	$(CC) $(CFLAGS) -o $(TRAVERSE_TEST) $(TRAVERSE_OBJS) $(LEX_OBJS) $(TRAVERSE_MAIN)

clean:
	rm -f *.o

