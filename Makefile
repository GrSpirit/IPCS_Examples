CC=gcc
all: multi shm_open tcp_server

shm_open:
	$(CC) -o shm_open shm_open.c -lrt
multi:
	$(CC) -o multi multi.c
tcp_server:
	$(CC) -o tcp_server tcp_server.c socket.c

clean:
	@rm -f shm_open multi tcp_server