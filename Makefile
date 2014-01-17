CC = cc
CFLAGS = -Wall
MULTI_OBJ = multi.o semaphore.o shared_memory.o
TCP_SERVER_OBJ = tcp_server.o socket.o queue.o

DEPS = message.h
all: multi tcp_server

multi: $(MULTI_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

tcp_server: $(TCP_SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f multi tcp_server *.o
