CC = cc
CFLAGS = -Wall
MULTI_OBJ = multi.o semaphore.o shared_memory.o
TCP_SERVER_OBJ = tcp_server.o socket.o queue.o

DEPS = message.h
all: multi tcp_server

multi: $(MULTI_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

tcp_server: $(TCP_SERVER_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -f multi tcp_server *.o
