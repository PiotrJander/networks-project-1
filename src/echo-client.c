#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"

#define BUFFER_SIZE 1000

void getaddrinfo_w(char *const *argv, struct addrinfo *addr_hints, struct addrinfo **addr_result);

void close_w(int sock);

void getaddrinfo_w(char *const *argv, struct addrinfo *addr_hints, struct addrinfo **addr_result);

void get_my_address(char *const *argv, struct addrinfo *addr_result, struct sockaddr_in *my_address);

int socket_w();

void connect_w(int sock, struct sockaddr_in *my_address);

int main(int argc, char *argv[]) {
    int sock;
    struct addrinfo addr_hints;
    struct addrinfo *addr_result;

    int i, flags, sflags;
    char buffer[BUFFER_SIZE];
    size_t len;
    ssize_t snd_len, rcv_len;
    struct sockaddr_in my_address;
    struct sockaddr_in srvr_address;
    socklen_t rcva_len;

    if (argc < 3) {
        fatal("Usage: %s host port message ...\n", argv[0]);
    }

    getaddrinfo_w(argv, &addr_hints, &addr_result);
    get_my_address(argv, addr_result, &my_address);
    sock = socket_w();
    connect_w(sock, &my_address);

    for (i = 3; i < argc; i++) {
        len = strnlen(argv[i], BUFFER_SIZE);
        if (len == BUFFER_SIZE) {
            (void) fprintf(stderr, "ignoring long parameter %d\n", i);
            continue;
        }
        (void) printf("sending to socket: %s\n", argv[i]);
        sflags = 0;
        rcva_len = (socklen_t) sizeof(my_address);
        snd_len = sendto(sock, argv[i], len, sflags,
                         (struct sockaddr *) &my_address, rcva_len);
        if (snd_len != (ssize_t) len) {
            syserr("partial / failed write");
        }

        (void) memset(buffer, 0, sizeof(buffer));
        flags = 0;
        len = (size_t) sizeof(buffer) - 1;
        rcva_len = (socklen_t) sizeof(srvr_address);
        rcv_len = recvfrom(sock, buffer, len, flags,
                           (struct sockaddr *) &srvr_address, &rcva_len);

        if (rcv_len < 0) {
            syserr("read");
        }
        (void) printf("read from socket: %zd bytes: %s\n", rcv_len, buffer);
    }

    close_w(sock);

    return 0;
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
            ((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
    (*my_address).sin_port = htons((uint16_t) atoi(argv[2])); // port from the command line

    freeaddrinfo(addr_result);
}

void close_w(int sock)
{
    if (close(sock) == -1) {
        syserr("close");
    }
}

void getaddrinfo_w(char *const *argv, struct addrinfo *addr_hints, struct addrinfo **addr_result)
{
    (void) memset(addr_hints, 0, sizeof(struct addrinfo));
    (*addr_hints).ai_family = AF_INET; // IPv4
    (*addr_hints).ai_socktype = SOCK_DGRAM;
    (*addr_hints).ai_protocol = IPPROTO_UDP;
    (*addr_hints).ai_flags = 0;
    (*addr_hints).ai_addrlen = 0;
    (*addr_hints).ai_addr = NULL;
    (*addr_hints).ai_canonname = NULL;
    (*addr_hints).ai_next = NULL;
    if (getaddrinfo(argv[1], NULL, addr_hints, addr_result) != 0) {
        syserr("getaddrinfo");
    }
}
