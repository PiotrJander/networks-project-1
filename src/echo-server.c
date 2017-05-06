#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "err.h"
#include "socket_wrappers.h"

static const int BUFFER_SIZE = 1000;
static const int PORT_NUM = 10001;


int main(int argc, char *argv[])
{
    struct sockaddr_in client_address;
    socklen_t snda_len = (socklen_t) sizeof(client_address);
    socklen_t rcva_len = (socklen_t) sizeof(client_address);
    char buffer[BUFFER_SIZE];

    int sock = socket_w(PF_INET, SOCK_DGRAM);

    struct sockaddr_in server_address;
    get_address(&server_address, INADDR_ANY, PORT_NUM);

    bind_w(sock, &server_address);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (;;) {
        ssize_t len = recvfrom_w(sock, buffer, BUFFER_SIZE, &client_address, &rcva_len);
        printf("read from socket: %zd bytes: %.*s\n", len, (int) len, buffer);

        sendto_w(sock, buffer, len, &client_address, snda_len);
    }
#pragma clang diagnostic pop
}
