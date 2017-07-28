/*
   Trying to write (very) simple stack structure in C.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct ITEM {
    int value;
    struct ITEM *next;
} ITEM;

typedef struct STACK {
    int size;
    ITEM *head;
    ITEM *tail;
} STACK;


STACK * stack_init() {
    STACK *stack = (STACK *)malloc(sizeof(STACK));

    stack->size = 0;
    stack->head = NULL;
    stack->tail = NULL;
 
    return stack;
}

int stack_size(STACK *stack) {
    return stack->size;
}

ITEM * stack_pop(STACK *stack) { 
    ITEM *item = stack->head;
    stack->head = stack->head->next;

    stack->size--;

    return item;
}

void stack_push(STACK *stack, ITEM *item) { 
   if(stack_size(stack) == 0) {
        stack->tail = item;
    } else {
        item->next = stack->head;
   }

    stack->head = item;
    stack->size++;
}

void stack_destroy(STACK *stack) {
    while (stack_size(stack) > 0) {
        ITEM *item = stack_pop(stack);
        free(item);
    }

    free(stack);
}

int main() {
    STACK *stack = stack_init();
    int i, size = 100;

    for(i=0; i < size; i++) {
        ITEM *item = (ITEM *)malloc(sizeof(ITEM)); 
        item->value = i;
        item->next = NULL;
        stack_push(stack, item);
    }

    printf("STACK SIZE: %d\n", stack->size);

    while(stack_size(stack) > 0) {
        ITEM *item = stack_pop(stack);
        printf("ITEM: %d\n", item->value);
    }

    // free up memory
    stack_destroy(stack);
}

