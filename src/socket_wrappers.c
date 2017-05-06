//
// Created by Piotr Jander on 06.05.2017.
//

#include "socket_wrappers.h"

//#include <unistd.h>
//#include <sys/socket.h>

#include "err.h"

ssize_t recv_w(int sock, char *buffer, size_t len)
{
    ssize_t rcv_len = recv(sock, buffer, len, 0);
    if (rcv_len < 0) {
        syserr("read");
    }
    return rcv_len;
}

void send_w(int sock, size_t len1, const char *string)
{
    ssize_t snd_len1 = send(sock, string, len1, 0);
    if (snd_len1 != (ssize_t) len1) {
        syserr("partial / failed write");
    }
}

void connect_w(int sock, struct sockaddr_in *my_address)
{
    socklen_t rcva_len1 = (socklen_t) sizeof((*my_address));
    int c = connect(sock, (struct sockaddr *) my_address, rcva_len1);
    if (c < 0) syserr("connect");
}

int socket_w()
{
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        syserr("socket");
    return sock;
}

void close_w(int sock)
{
    if (close(sock) == -1) {
        syserr("close");
    }
}
