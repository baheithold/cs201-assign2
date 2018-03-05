/*
 *  File:   queue.c
 *  Author: Brett Heithold
 */

#include "queue.h"
#include "sll.h"
#include <stdlib.h>
#include <assert.h>

#define FRONT 0

struct QUEUE {
    SLL *store;
    int size;

    void (*display)(void *, FILE *);
    void (*free)(void *);
};

QUEUE *newQUEUE(void (*d)(void *, FILE *), void (*f)(void *)) {
    QUEUE *newQ = malloc(sizeof(QUEUE));
    assert(newQ != 0);
    newQ->store = newSLL(d, f);
    newQ->size = 0;
    newQ->display = d;
    newQ->free = f;
    return newQ;
}

void enqueue(QUEUE *items, void *value) {
    assert(items != 0);
    insertSLL(items->store, sizeSLL(items->store), value);
    items->size++;
}

void *dequeue(QUEUE *items) {
    assert(items->size > 0);
    items->size--;
    return removeSLL(items->store, FRONT);
}

void *peekQUEUE(QUEUE *items) {
    assert(items->size > 0);
    return getSLL(items->store, FRONT);
}

void displayQUEUE(QUEUE *items, FILE *fp) {
    assert(items != 0);
    fprintf(fp, "<");
    for (int i = 0; i < sizeSLL(items->store); i++) {
        items->display(getSLL(items->store, i), fp);
        if (i < sizeSLL(items->store) - 1) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, ">");
}

int sizeQUEUE(QUEUE *items) {
    assert(items != 0);
    return items->size;
}

void displayQUEUEdebug(QUEUE *items, FILE *fp) {
    assert(items != 0);
    displaySLLdebug(items->store, fp);
}

void freeQUEUE(QUEUE *items) {
    freeSLL(items->store);
    free(items);
}
