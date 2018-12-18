//
// Created by Administrator on 2018/12/12.
//

#ifndef ALGORITHM_LIST_QUEUE_H
#define ALGORITHM_LIST_QUEUE_H

typedef struct _list_queue_node{
    void *data;
    struct _list_queue_node *next;
}queue_node;

typedef struct _list_queue{
    int num;
    queue_node *head;
    queue_node *tail;
}list_queue;

#define list_queue_is_empty(queue) ((queue->num) == 0)

list_queue *list_queue_create();
int list_queue_enqueue(list_queue *queue, void *data);
int list_queue_dequeue(list_queue *queue, void **data);

#endif //ALGORITHM_LIST_QUEUE_H
