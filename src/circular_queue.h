//
// Created by Piotr Jander on 08.05.2017.
//

#ifndef ZADANIE1_CIRCULAR_QUEUE_H
#define ZADANIE1_CIRCULAR_QUEUE_H

typedef struct cqueue {
    char **queue;
    int queue_len;
    int element_len;
    int read_counter;
    int write_counter;
} CQueue;

void cqueue_new(CQueue *q, const size_t queue_len, const size_t element_len, char **queue);

void
cqueue_enqueue(CQueue *q, char *src);

void
cqueue_dequeue(CQueue *q, char *dst);

int
cqueue_is_empty(CQueue *q);

#endif //ZADANIE1_CIRCULAR_QUEUE_H
