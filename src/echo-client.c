#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"

#define BUFFER_SIZE 1000

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


    if (argc < 4 || argc > 5) {
        fatal("Usage: %s timestamp c host [port]\n", argv[0]);
    }
    // validate timestamp
    uint64_t timestamp = (uint64_t) strtoll(argv[1], NULL, 10);
    if (timestamp >= 71728934400LL) {
        fatal("Year is greater than 4242\n");
    }
//    // validate c
//    if (strlen(argv[2]) > 1) {
//        fatal("The second argument c must be a single character");
//    }


    // 'converting' host/port in string to struct addrinfo
//    (void) memset(&addr_hints, 0, sizeof(struct addrinfo));
    memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; // IPv4
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = 0;
    addr_hints.ai_addrlen = 0;
    addr_hints.ai_addr = NULL;
    addr_hints.ai_canonname = NULL;
    addr_hints.ai_next = NULL;
    if (getaddrinfo(argv[3], NULL, &addr_hints, &addr_result) != 0) {
        syserr("getaddrinfo");
    }

    // maybe override port
    uint16_t port = argc == 4 ? (uint16_t) atoi(argv[4]) : (uint16_t) 20160;

    my_address.sin_family = AF_INET; // IPv4
    my_address.sin_addr.s_addr =
            ((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
    my_address.sin_port = htons(port); // port from the command line

    freeaddrinfo(addr_result);

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        syserr("socket");

    socklen_t rcva_len1 = (socklen_t) sizeof(my_address);
    int c = connect(sock, (struct sockaddr *) &my_address, rcva_len1);
    if (c < 0) syserr("connect");

//    send(sock, "hello", 6, 0);
//    char bufferek[6];
//    recv(sock, bufferek, 6, 0);
//    printf("%s", bufferek);

    for (i = 1; i < 2; i++) {
        len = strnlen(argv[i], BUFFER_SIZE);
        if (len == BUFFER_SIZE) {
            (void) fprintf(stderr, "ignoring long parameter %d\n", i);
            continue;
        }
        (void) printf("sending to socket: %s\n", argv[i]);
        sflags = 0;
        rcva_len = (socklen_t) sizeof(my_address);
        snd_len = send(sock, argv[i], len, sflags);
        if (snd_len != (ssize_t) len) {
            syserr("partial / failed write");
        }

        (void) memset(buffer, 0, sizeof(buffer));
        flags = 0;
        len = (size_t) sizeof(buffer) - 1;
        rcva_len = (socklen_t) sizeof(srvr_address);
        rcv_len = recv(sock, buffer, len, flags);

        if (rcv_len < 0) {
            syserr("read");
        }
        (void) printf("read from socket: %zd bytes: %s\n", rcv_len, buffer);
    }
    
    if (close(sock) == -1) { //very rare errors can occur here, but then
        syserr("close"); //it's healthy to do the check
    }

    return 0;
}
