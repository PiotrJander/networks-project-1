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

int socket_w();

void connect_w(int sock, struct sockaddr_in *my_address);

void send_w(int sock, size_t len1, const char *string);

ssize_t recv_w(int sock, char *buffer, size_t len);

void close_w(int sock);

#endif //ZADANIE1_SOCKET_WRAPPERS_H
