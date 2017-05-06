#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <inttypes.h>

#include "err.h"
#include "socket_wrappers.h"

static const size_t BUFFER_SIZE = sizeof(uint64_t) + sizeof(char);
static const uint16_t DEFAULT_PORT = (uint16_t) 20160;

void getaddrinfo_w(const char *host, struct addrinfo **addr_result);

void validate(int argc, char **argv, uint64_t *timestamp, char *character, char **host, uint16_t *port);

int main(int argc, char *argv[])
{
    // validate
    uint64_t timestamp_h;
    char character;
    char *host;
    uint16_t port;
    validate(argc, argv, &timestamp_h, &character, &host, &port);

    struct addrinfo *addr_result;
    getaddrinfo_w(host, &addr_result);
    freeaddrinfo(addr_result);

    struct sockaddr_in my_address;
    in_addr_t s_addr = ((struct sockaddr_in *) (addr_result->ai_addr))->sin_addr.s_addr;
    get_address(&my_address, s_addr, port);

    int sock = socket_w(PF_INET, SOCK_DGRAM);
    connect_w(sock, &my_address);

    // write data to buffer
    char buffer[BUFFER_SIZE];
    uint64_t timestamp_n = htonll(timestamp_h);
    memcpy(&timestamp_n, buffer, sizeof(uint64_t));
    buffer[BUFFER_SIZE - 1] = character;

    // print data
    printf("timestamp h: %llu\n", timestamp_h);
    printf("timestamp n: %llu\n", timestamp_n);
    uint64_t timestamp_p;
    memcpy(&timestamp_p, buffer, sizeof(uint64_t));
    printf("timestamp n from buffer: %llu\n", timestamp_p);
    printf("character: %c\n", buffer[BUFFER_SIZE - 1]);

    send_w(sock, buffer, BUFFER_SIZE);
    printf("sending to socket: %s\n", buffer);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (;;) {
        memset(buffer, 0, sizeof(buffer));
        size_t len = (size_t) sizeof(buffer) - 1;
        recv_w(sock, buffer, len);
        printf("%s", buffer);  // TODO maybe use write here
    }
#pragma clang diagnostic pop

//    close_w(sock);
//    return 0;
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

void validate(int argc, char **argv, uint64_t *timestamp, char *character, char **host, uint16_t *port)
{
    if (argc < 4 || argc > 5) {
        fatal("Usage: %s timestamp char host [port]\n", argv[0]);
    }

    // validate timestamp
    *timestamp = (uint64_t) strtoll(argv[1], NULL, 10);
    if (*timestamp >= 71728934400LL) {
        fatal("Year is greater than 4242\n");
    }

    // validate c
    if (strlen(argv[2]) > 1) {
        fatal("The second argument c must be a single character");
    } else {
        *character = argv[2][0];
    }

    // set host
    *host = argv[3];

    // maybe override port
    *port = argc == 5 ? (uint16_t) atoi(argv[4]) : DEFAULT_PORT;
}
