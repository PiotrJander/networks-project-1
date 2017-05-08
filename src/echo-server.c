#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "socket_wrappers.h"
#include "err.h"
#include "circular_queue.h"

static const size_t BIG_BUFFER = 65535;
static const size_t SMALL_BUFFER = sizeof(uint64_t) + sizeof(char);  // 9
static const size_t QUEUE_LEN = 4096;

void validate(int argc, char **argv, uint16_t *port, FILE **file);

int copy_file_to_buffer(FILE *file, char *buffer);

int main(int argc, char *argv[])
{
    // validate
    uint16_t port;
    FILE *file;
    validate(argc, argv, &port, &file);

    // allocate buffers
    char small_buffer[SMALL_BUFFER];
    char big_buffer[BIG_BUFFER];

    // prepare the queue
//    char *cqueue_queue[QUEUE_LEN];
    char *cqueue_queue[2];
    CQueue cqueue;
//    cqueue_new(&cqueue, QUEUE_LEN, SMALL_BUFFER, (char **) cqueue_queue);
    cqueue_new(&cqueue, 2, SMALL_BUFFER, (char **) cqueue_queue);

    // copy file to buffer
    int message_len = copy_file_to_buffer(file, big_buffer);

    // client address variables
    struct sockaddr_in client_address;
    socklen_t rcva_len = (socklen_t) sizeof(client_address);
    socklen_t snda_len = (socklen_t) sizeof(client_address);

    // poll array
    struct pollfd server[0];
    server[0].fd = socket_w(AF_INET, SOCK_DGRAM);  // socket
//    server[0].events = POLLIN | POLLOUT;
    server[0].events = POLLIN;
    server[0].revents = 0;

    // make address, listen on all interfaces
    struct sockaddr_in server_address;
    get_address(&server_address, INADDR_ANY, port);

    // bind
    bind_w(server[0].fd, &server_address);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    while (1) {

        int i = poll_w(server, 1, -1);

        if (server[0].revents & POLLIN) {
            // we can read
            // TODO write to buffer
            ssize_t recv_len = recvfrom_w(server[0].fd, small_buffer, SMALL_BUFFER,
                                          &client_address, &rcva_len);

            if (recv_len == SMALL_BUFFER) {
                // copy to queue here
                c_enqueue(&cqueue, small_buffer);
            } else {
                // ignore invalid (too short) datagrams
                fprintf(stderr, "Received an invalid datagram of length less than 9\n");
            }

//            if (server[0].revents & POLLOUT) {
//                // we can write
//                sendto_w(server[0].fd, buffer, message_len, &client_address, snda_len);
//            }
        }

        // reset the revents field
        server[0].revents = 0;

    }

#pragma clang diagnostic pop

}

/**
 * Copies the file to buffer, starting at the offset. Error if file too long.
 */
int copy_file_to_buffer(FILE *file, char *buffer)
{
    int i;
    for (i = SMALL_BUFFER; i < BIG_BUFFER; ++i) {
        buffer[i] = (char) fgetc(file);
        if (feof(file)) {
            // file is assumed to have no NULL chars, but we insert one at the end
            buffer[i] = '\0';
            break;
        }
    }
    if (i == BIG_BUFFER) {
        fatal("File size greater than %d - %d bytes", BIG_BUFFER, SMALL_BUFFER);
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
