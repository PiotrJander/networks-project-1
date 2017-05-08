#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"
#include "socket_wrappers.h"

#define htonll_(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))

static const size_t SEND_BUFFER_SIZE = sizeof(uint64_t) + sizeof(char);
static const int RECV_BUFFER_SIZE = 65535;
static const uint16_t DEFAULT_PORT = (uint16_t) 20160;

void validate(int argc, char **argv, uint64_t *timestamp, char *character, char **host, uint16_t *port);

int main(int argc, char *argv[])
{
    // validate
    uint64_t timestamp_h;
    char character;
    char *host;
    uint16_t port;
    validate(argc, argv, &timestamp_h, &character, &host, &port);

    // addrinfo
    struct addrinfo *addr_result;
    getaddrinfo_w(host, &addr_result);
    freeaddrinfo(addr_result);

    // make address
    struct sockaddr_in my_address;
    in_addr_t s_addr = ((struct sockaddr_in *) (addr_result->ai_addr))->sin_addr.s_addr;
    get_address(&my_address, s_addr, port);

    // socket and connect
    int sock = socket_w(PF_INET, SOCK_DGRAM);
    connect_w(sock, &my_address);

    // write data to buffer
    char send_buffer[SEND_BUFFER_SIZE];
    uint64_t timestamp_n = htonll_(timestamp_h);
    memcpy(send_buffer, &timestamp_n, sizeof(uint64_t));
    send_buffer[sizeof(uint64_t)] = character;

    // send
    send_w(sock, send_buffer, SEND_BUFFER_SIZE);

    // recv buffer decl
    char recv_buffer[RECV_BUFFER_SIZE];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    // listen to server in infinite loop
    for (;;) {
        // recv
        ssize_t recv_len = recv_w(sock, recv_buffer, RECV_BUFFER_SIZE);

        // ignore invalid (too short) datagrams
        if (recv_len < SEND_BUFFER_SIZE) {
            fprintf(stderr, "Received an invalid datagram of length less than 9\n");
            continue;
        }

        // parse timestamp
        uint64_t timestamp_rcv;
        memcpy(&timestamp_rcv, recv_buffer, sizeof(uint64_t));
        timestamp_rcv = ntohll(timestamp_rcv);

        // print message
        printf("%llu%s", timestamp_rcv, recv_buffer + sizeof(uint64_t));
    }

#pragma clang diagnostic pop

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
