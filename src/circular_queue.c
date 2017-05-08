//
// Created by Piotr Jander on 08.05.2017.
//

#include <string.h>

#include "circular_queue.h"

void cqueue_new(CQueue *q, const size_t queue_len, const size_t element_len, char **queue)
{
    q->queue = queue;
    q->queue_len = (int) queue_len;
    q->element_len = (int) element_len;
    q->read_counter = 0;
    q->write_counter = 0;
}

void c_enqueue(CQueue *q, char *src)
{
    strncpy(q->queue[q->write_counter], src, q->element_len);
    q->write_counter = (q->write_counter + 1) % q->queue_len;
}

void c_dequeue(CQueue *q, char *dst)
{
    strncpy(dst, q->queue[q->read_counter], q->element_len);
    q->read_counter = (q->read_counter + 1) % q->queue_len;
}

int c_is_empty(CQueue *q)
{
    return q->read_counter == q->write_counter;
}
