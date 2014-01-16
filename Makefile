CC=gcc
all: multi tcp_server

multi:
	$(CC) -o multi multi.c
tcp_server:
	$(CC) -o tcp_server tcp_server.c socket.c

clean:
	@rm -f multi tcp_server
