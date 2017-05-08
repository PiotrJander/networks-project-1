//
// Created by Piotr Jander on 06.05.2017.
//

#ifndef ZADANIE1_SOCKET_WRAPPERS_H
#define ZADANIE1_SOCKET_WRAPPERS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <time.h>
#include "err.h"


void getaddrinfo_w(const char *host, struct addrinfo **addr_result)
{
    struct addrinfo addr_hints;
    memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; // IPv4
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = 0;
    addr_hints.ai_addrlen = 0;
    addr_hints.ai_addr = NULL;
    addr_hints.ai_canonname = NULL;
    addr_hints.ai_next = NULL;
    if (getaddrinfo(host, NULL, &addr_hints, addr_result) != 0) {
        syserr("getaddrinfo");
    }
}

void get_address(struct sockaddr_in *address, in_addr_t s_addr, uint16_t port)
{
    (*address).sin_family = AF_INET;
    (*address).sin_addr.s_addr = s_addr;  // address IP / listening on all interfaces
    (*address).sin_port = htons(port);
}

int socket_w(int domain, int type)
{
    int sock = socket(domain, type, 0);
    if (sock < 0)
        syserr("socket");
    return sock;
}

int poll_w(struct pollfd *server, nfds_t nfds, int timeout)
{
    int ret = poll(server, nfds, timeout);
    if (ret < 0)
        perror("poll");
    return ret;
}

void bind_w(int sock, struct sockaddr_in *server_address)
{
    if (bind(sock, (struct sockaddr *) server_address, (socklen_t) sizeof((*server_address))) < 0) {
        syserr("bind");
    }
}

void connect_w(int sock, struct sockaddr_in *my_address)
{
    socklen_t rcva_len1 = (socklen_t) sizeof((*my_address));
    int c = connect(sock, (struct sockaddr *) my_address, rcva_len1);
    if (c < 0) syserr("connect");
}

void send_w(int sock, const char *string, size_t len1)
{
    ssize_t snd_len1 = send(sock, string, len1, 0);
    if (snd_len1 != (ssize_t) len1) {
        syserr("partial / failed write");
    }
}

ssize_t sendto_w(int sock, const char *buffer, ssize_t len, struct sockaddr_in *client_address, socklen_t snda_len)
{
    ssize_t snd_len = sendto(sock, buffer, (size_t) len, 0,
                             (struct sockaddr *) client_address, snda_len);
    if (snd_len != len) {
        syserr("error on sending datagram to client socket");
    }
    return snd_len;
}

ssize_t recv_w(int sock, char *buffer, size_t len)
{
    ssize_t rcv_len = recv(sock, buffer, len, 0);
    if (rcv_len < 0) {
        syserr("read");
    }
    return rcv_len;
}

ssize_t recvfrom_w(int sock, char buffer[], size_t buffer_len, struct sockaddr_in *client_address, socklen_t *rcva_len)
{
    ssize_t len = recvfrom(sock, buffer, buffer_len, 0,
                           (struct sockaddr *) client_address, rcva_len);
    if (len < 0) {
        syserr("error on datagram from client socket");
    }
    return len;
}

void close_w(int sock)
{
    if (close(sock) == -1) {
        syserr("close");
    }
}

#endif //ZADANIE1_SOCKET_WRAPPERS_H
