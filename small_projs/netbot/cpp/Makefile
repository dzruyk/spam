CC=g++
#CC=x86_64-w64-mingw32-g++
CFLAGS= -Wall -g

TARGET=my_netbot
OBJS=main.o sock.o tcp_client.o coreinstall.o common.o

all: $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *~ *.o $(TARGET)
