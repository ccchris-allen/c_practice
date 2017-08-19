/*
   Trying to write (very) simple queue structure in C.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct ITEM {
    int value;
    struct ITEM *next;
} ITEM;

typedef struct QUEUE {
    int size;
    ITEM *head;
    ITEM *tail;
} QUEUE;


QUEUE * q_init() {
    QUEUE *q = (QUEUE *)malloc(sizeof(QUEUE));

    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
 
    return q;
}

int q_size(QUEUE *q) {
    return q->size;
}

ITEM * q_pop(QUEUE *q) { 
    ITEM *item = q->head;
    q->head = q->head->next;

    q->size--;

    return item;
}

void q_push(QUEUE *q, ITEM *item) { 
   if(q_size(q) == 0) {
        q->head = item;
    } else {
        q->tail->next = item;
   }

    q->tail = item;
    q->size++;
}

void q_destroy(QUEUE *q) {
    while (q_size(q) > 0) {
        ITEM *item = q_pop(q);
        free(item);
    }

    free(q);
}



// Test this out...
int main() {
    QUEUE *q = q_init();
    int i;
    int size = 100;

    for(i=0; i < size; i++) {
        // memory management 101: 
        // this will not work since local, non-malloced structs will keep overriding at same memory location
        // in stack (whereas malloced are in the heap)
        //ITEM item = { i, NULL };
        //q_push(q, &item);

        ITEM *item = (ITEM *)malloc(sizeof(ITEM)); 
        item->value = i;
        item->next = NULL;
        q_push(q, item);
    }

    printf("QUEUE SIZE: %d\n", q->size);

    while(q_size(q) > 0) {
        ITEM *item = q_pop(q);
        printf("ITEM: %d\n", item->value);
        free(item);
    }

    // free up memory
    q_destroy(q);
}

