//
// Created by Piotr Jander on 08.05.2017.
//

#ifndef ZADANIE1_CLIENT_LIST_H
#define ZADANIE1_CLIENT_LIST_H

#include <netinet/in.h>

typedef struct client {
    struct sockaddr_in address;
    time_t last_access;
} Client;

typedef struct client_list {
    Client **clients;
    int len;
} ClientList;

void client_list_make(ClientList *client_list, Client **clients, int len);

void client_list_add(ClientList *client_list, struct sockaddr_in *address);

#endif //ZADANIE1_CLIENT_LIST_H
