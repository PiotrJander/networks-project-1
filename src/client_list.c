//
// Created by Piotr Jander on 08.05.2017.
//

#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "client_list.h"


bool
address_eq(const struct sockaddr_in *address, const Client *client);

static time_t
time_w();

void client_list_make(ClientList *client_list, Client **clients, int len)
{
    client_list->clients = clients;
    client_list->len = len;

    for (int i = 0; i < len; ++i) {
        clients[i] = NULL;
    }
}

void client_list_add(ClientList *client_list, struct sockaddr_in *address)
{
    Client **clients = client_list->clients;
    int oldest_client_idx = 0;
    int free_slot_idx = -1;

    for (int i = 0; i < client_list->len; ++i) {
        Client *client = clients[i];
        if (client == NULL) {
            // save the empty slot
            free_slot_idx = i;
        } else if (address_eq(address, client)) {
            // returning client; update time
            client->last_access = time_w();
            return;
        } else if (!clients[oldest_client_idx] || client->last_access < clients[oldest_client_idx]->last_access) {
            // this is the oldest client so far
            oldest_client_idx = i;
        }
    }
    // else, if we haven't returned yet,
    if (free_slot_idx == -1) {
        // no empty slots; replace oldest client with the new one
        int idx = oldest_client_idx;
        clients[idx]->address = *address;
        clients[idx]->last_access = time_w();
    } else {
        // use the empty slot
        int idx = free_slot_idx;
        clients[idx] = malloc(sizeof(Client));
        clients[idx]->address = *address;
        clients[idx]->last_access = time_w();
    }
}

bool address_eq(const struct sockaddr_in *address, const Client *client)
{ return client->address.sin_port == address->sin_port && client->address.sin_addr.s_addr == address->sin_addr.s_addr; }

static time_t time_w()
{
    time_t ret = time(NULL);
    if (ret == -1) {
        perror("time");
        exit(1);
    } else {
        return ret;
    }
}