#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"

#define BUFFER_SIZE 1000

void getaddrinfo_w(char *const *argv, struct addrinfo **addr_result);

void close_w(int sock);

void getaddrinfo_w(char *const *argv, struct addrinfo **addr_result);

void get_my_address(char *const *argv, struct addrinfo *addr_result, struct sockaddr_in *my_address);

int socket_w();

void connect_w(int sock, struct sockaddr_in *my_address);

void send_w(int sock, size_t len1, const char *string);

ssize_t recv_w(int sock, char *buffer, size_t len);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fatal("Usage: %s host port message ...\n", argv[0]);
    }

    struct addrinfo *addr_result;
    getaddrinfo_w(argv, &addr_result);

    struct sockaddr_in my_address;
    get_my_address(argv, addr_result, &my_address);

    int sock = socket_w();
    connect_w(sock, &my_address);

    char buffer[BUFFER_SIZE];

    for (int i = 3; i < argc; i++) {

        size_t len1 = strnlen(argv[i], BUFFER_SIZE);
        if (len1 == BUFFER_SIZE) {
            fatal("Buffer overflow");
        }
        send_w(sock, len1, argv[i]);

        printf("sending to socket: %s\n", argv[i]);

        memset(buffer, 0, sizeof(buffer));
        size_t len = (size_t) sizeof(buffer) - 1;
        ssize_t rcv_len = recv_w(sock, buffer, len);

        printf("read from socket: %zd bytes: %s\n", rcv_len, buffer);
    }

    close_w(sock);

    return 0;
}

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

void get_my_address(char *const *argv, struct addrinfo *addr_result, struct sockaddr_in *my_address)
{
    (*my_address).sin_family = AF_INET; // IPv4
    (*my_address).sin_addr.s_addr =
            ((struct sockaddr_in *) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
    (*my_address).sin_port = htons((uint16_t) atoi(argv[2])); // port from the command line

    freeaddrinfo(addr_result);
}

void close_w(int sock)
{
    if (close(sock) == -1) {
        syserr("close");
    }
}

void getaddrinfo_w(char *const *argv, struct addrinfo **addr_result)
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
    if (getaddrinfo(argv[1], NULL, &addr_hints, addr_result) != 0) {
        syserr("getaddrinfo");
    }
}
