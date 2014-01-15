/* socket.h */
#ifndef SOCKET
#define SOCKET

int recvall(int sockfd, void *buf, int buflen);
int sendall(int sockfd, void *buf, int len);

#endif