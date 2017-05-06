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

int socket_w(int domain, int type);

void bind_w(int sock, struct sockaddr_in *server_address);

void connect_w(int sock, struct sockaddr_in *my_address);

void send_w(int sock, size_t len1, const char *string);

ssize_t sendto_w(int sock, const char *buffer, ssize_t len, struct sockaddr_in *client_address, socklen_t snda_len);

ssize_t recv_w(int sock, char *buffer, size_t len);

ssize_t recvfrom_w(int sock, char buffer[], size_t buffer_len, struct sockaddr_in *client_address, socklen_t *rcva_len);

void close_w(int sock);

void get_address(struct sockaddr_in *address, in_addr_t s_addr, uint16_t port);

#endif //ZADANIE1_SOCKET_WRAPPERS_H
