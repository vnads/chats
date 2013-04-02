CC=/usr/bin/g++
#CC=/usr/bin/g++
CFLAGS= -g -w -I.
LINKFLAGS= -g -w

all: clean chat

chat: chat.o message.o
	$(CC) $(LINKFLAGS) -o chat chat.o message.o -lpthread

chat.o: chat.cc message.hh
	$(CC) $(CFLAGS) -c chat.cc -o chat.o

message.o: message.cc message.hh
	$(CC) $(CFLAGS) -c message.cc -o message.o

clean:
	rm -f *.o chat *.dat
