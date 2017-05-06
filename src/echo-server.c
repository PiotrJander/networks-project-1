#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socket_wrappers.h"
#include "err.h"

static const int BUFFER_SIZE = 534;
static const int BUFFER_OFFSET = sizeof(uint64_t) + sizeof(char);  // 9

void validate(int argc, char **argv, uint16_t *port, FILE **file);

int copy_file_to_buffer(FILE *file, char *buffer);

int main(int argc, char *argv[])
{
    uint16_t port;
    FILE *file;
    validate(argc, argv, &port, &file);

    char buffer[BUFFER_SIZE];

    int len = copy_file_to_buffer(file, buffer);

    struct sockaddr_in client_address;
    socklen_t rcva_len = (socklen_t) sizeof(client_address);
    socklen_t snda_len = (socklen_t) sizeof(client_address);

    int sock = socket_w(AF_INET, SOCK_DGRAM);

    struct sockaddr_in server_address;
    get_address(&server_address, INADDR_ANY, port);

    bind_w(sock, &server_address);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (;;) {
        ssize_t recv_len = recvfrom_w(sock, buffer, (size_t) BUFFER_OFFSET,
                                      &client_address, &rcva_len);

        // TODO expect recv_len = 9, otherwise error

        sendto_w(sock, buffer, len, &client_address, snda_len);
    }
#pragma clang diagnostic pop
}

int copy_file_to_buffer(FILE *file, char *buffer)
{
    int i;
    for (i = BUFFER_OFFSET; i < BUFFER_SIZE - 2; ++i) {
        buffer[i] = (char) fgetc(file);
        if (feof(file)) {
            // file is assumed to have no NULL chars, but we insert one at the end
            buffer[i] = '\0';
            break;
        }
    }
    if (i == BUFFER_SIZE - 1) {
        fatal("File size greater than %d - %d - 1 bytes", BUFFER_SIZE, BUFFER_OFFSET);
    }
    fclose(file);

    // constant length of message in the buffer is i + 1
    return i + 1;
}

void validate(int argc, char **argv, uint16_t *port, FILE **file)
{
    if (argc != 3) {
        fatal("Usage: %s port filename\n", argv[0]);
    }

    *port = (uint16_t) atoi(argv[1]);

    *file = fopen(argv[2], "r");
    if (*file == NULL) {
        fatal("Invalid path to file");
    }
}
