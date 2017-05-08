//
// Created by Piotr Jander on 08.05.2017.
//

#include <string.h>
#include <stdlib.h>

#include "circular_queue.h"

void cqueue_new(CQueue *q, const size_t queue_len, const size_t element_len, char **queue)
{
    q->queue = queue;
    q->queue_len = (int) queue_len;
    q->element_len = (int) element_len;
    q->read_counter = 0;
    q->write_counter = 0;
}

void cqueue_enqueue(CQueue *q, char *src)
{
    char *element = malloc(sizeof(char) * q->element_len);
    strncpy(element, src, q->element_len);
    q->queue[q->write_counter] = element;
    q->write_counter = (q->write_counter + 1) % q->queue_len;
}

void cqueue_dequeue(CQueue *q, char *dst)
{
    char *element = q->queue[q->read_counter];
    strncpy(dst, element, q->element_len);
    free(element);
    q->read_counter = (q->read_counter + 1) % q->queue_len;
}

int cqueue_is_empty(CQueue *q)
{
    return q->read_counter == q->write_counter;
}
