/* socket.c */
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

int recvall(int sockfd, void *buf, int buflen)
{
	int bytes_read;
	int total = 0;
	while(1) {
		bytes_read = recv(sockfd, buf, buflen, 0);
		total += bytes_read;
	    if(bytes_read <= 0) break;
	}
	return total;
}

int sendall(int sockfd, void *buf, int len)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(sockfd, buf + total, len - total, 0);
        if(n == -1) { break; }
        total += n;
    }

    return (n==-1 ? -1 : total);
}
