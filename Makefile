CC=cc
CFLAGS=-Wall
all: multi tcp_server

multi: multi.o semaphore.o shared_memory.o
	$(CC) -o multi multi.o semaphore.o shared_memory.o

tcp_server: tcp_server.o socket.o
	$(CC) -o tcp_server socket.o tcp_server.o

clean:
	@rm -f multi tcp_server *.o
