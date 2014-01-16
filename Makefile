CC=cc
CFLAGS=-Wall
all: multi tcp_server

multi: multi.o
	$(CC) -o multi multi.o

#multi.o: multi.h
#	$(CC) $(CFLAGS) multi.c

tcp_server: tcp_server.o socket.o
	$(CC) -o tcp_server socket.o tcp_server.o

#tcp_server.o socket.o: tcp_server.h socket.h

#tcp_server.o: tcp_server.c
#	$(CC) $(CFLAGS) tcp_server.c

#socket.o: socket.c
#	$(CC) $(CFLAGS) socket.c

clean:
	@rm -f multi tcp_server *.o
