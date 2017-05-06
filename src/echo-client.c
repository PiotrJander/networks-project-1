#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"
#include "socket_wrappers.h"

static const int BUFFER_SIZE = 1000;

void getaddrinfo_w(const char *host, struct addrinfo **addr_result);

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fatal("Usage: %s host port message ...\n", argv[0]);
    }

    struct addrinfo *addr_result;
    getaddrinfo_w(argv[1], &addr_result);
    freeaddrinfo(addr_result);

    struct sockaddr_in my_address;
    in_addr_t s_addr = ((struct sockaddr_in *) (addr_result->ai_addr))->sin_addr.s_addr;
    uint16_t port = (uint16_t) atoi(argv[2]);
    get_address(&my_address, s_addr, port);

    int sock = socket_w(PF_INET, SOCK_DGRAM);
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
