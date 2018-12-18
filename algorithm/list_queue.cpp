//
// Created by Administrator on 2018/12/12.
//
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list_queue.h"

/**
 * create queue
 * @return list_queue if succeed, nullptr if failed
 */
list_queue *list_queue_create() {
    list_queue *queue = nullptr;

    queue = (list_queue *) malloc(sizeof(list_queue));
    if(queue == nullptr) {
        return nullptr;
    }
    queue->num = 0;
    queue->head = nullptr;
    queue->tail = nullptr;
    return queue;
}

/**
 * enqueue a node
 * @param queue
 * @param data
 * @return 0 if succeed, -1 if failed
 */
int list_queue_enqueue(list_queue *queue, void *data) {
    queue_node *ptmp = nullptr;
    if (queue == nullptr) {
        return -1;
    }
    ptmp = static_cast<queue_node *>(malloc(sizeof(queue_node)));
    if (ptmp == nullptr) {
        return -1;
    }
    ptmp->data = data;
    ptmp->next = nullptr;
    if (queue->head == nullptr) {
        queue->head = ptmp;
    } else {
        queue->tail->next = ptmp;
    }
    queue->tail = ptmp;
    queue->num++;
    return 0;
}

/**
 * dequeue a node
 * @param queue
 * @param data
 * @return 0 if succeed, 1 if nullptr
 */
int list_queue_dequeue(list_queue *queue, void **data) {
    queue_node *ptmp = nullptr;
    if ((queue == nullptr) || (data == nullptr) || (list_queue_is_empty(queue))) { // need to determine if queue is empty
        return -1;
    }
    *data = queue->head->data;
    ptmp = queue->head;
    queue->head = queue->head->next;
    queue->num--;
    //if queue contains only one node, head and tail will both point to it, after dequeue, should also move tail
    if (queue->head == nullptr) {
        queue->tail = nullptr;
    }
    free(ptmp);
    return 0;
}

