/*
 *  File:   stack.c
 *  Author: Brett Heithold
 *  Description: This is the implementation file for the stack class.
 */

#include "stack.h"
#include "dll.h"
#include <stdlib.h>
#include <assert.h>

#define TOP 0


/*
 *  Type:   STACK
 *  Description: The basis of the STACK class.
 */
struct STACK {
    DLL *store;
    int size;

    void (*display)(void *, FILE *);
    void (*free)(void *);
};


/*
 *  Constructor:    newSTACK
 *  Usage:  STACK *s = newSTACK(displayINTEGER, freeINTEGER);
 *  Description: The constructor is passed functions that knows how to display
 *  and free the generic values stored in the queue. 
 */
STACK *newSTACK(void (*d)(void *, FILE *), void (*f)(void *)) {
    STACK *s = malloc(sizeof(STACK));
    assert(s != 0);
    s->store = newDLL(d, f);
    s->size = 0;
    s->display = d;
    s->free = f;
    return s;
}


/*
 *  Method: push
 *  Usage:  push(items, value);
 *  Description: The push method runs in constant or amortized constant time. 
 *  The value to be pushed is stored in the underlying data structure. 
 */
void push(STACK *items, void *value) {
    assert(items != 0);
    insertDLL(items->store, TOP, value);
    items->size++;
}


/*
 *  Method: pop
 *  Usage:  void *value = pop(s);
 *  Description: The pop method runs in constant or amortized constant time.
 *  The value to be popped is removed in the underlying data structure. 
 */
void *pop(STACK *items) {
    assert(sizeDLL(items->store) > 0);
    items->size--;
    return removeDLL(items->store, TOP);
}


/*
 *  Method: peekSTACK
 *  Usage:  void *top = peekSTACK(s);
 *  Description: The peek method returns the value ready to come off the stack,
 *  but leaves the stack unchanged. It runs in constant time. 
 */
void *peekSTACK(STACK *items) {
    assert(items != 0);
    assert(sizeDLL(items->store) > 0);
    return getDLL(items->store, TOP);
}


/*
 *  Method: sizeSTACK
 *  Usage int size = sizeSTACK(s);
 *  Description: The size method returns the number of items stored in
 *  the stack. It runs in amortized constant time. 
 */
int sizeSTACK(STACK *items) {
    assert(items != 0);
    return items->size;
}


/*
 *  Method: displaySTACK
 *  Usage:  displaySTACK(items, stdout);
 *  Example Output: |1,9,2,6,5| where 1 is on the top of the stack
 */
void displaySTACK(STACK *items, FILE *fp) {
    assert(items != 0);
    fprintf(fp, "|");
    for (int i = 0; i < sizeDLL(items->store); i++) {
        items->display(getDLL(items->store, i), fp);
        if (i < sizeDLL(items->store) - 1) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "|");
}


/*
 *  Method: displaySTACKdebug
 *  Usage:  displaySTACKdebug(items, stdout);
 *  Description: This method calls the debug method of the underlying
 *  data structure.
 */
void displaySTACKdebug(STACK *items, FILE *fp) {
    assert(items != 0);
    displayDLLdebug(items->store, fp);
}

/*
 *  Method: freeSTACK
 *  Usage:  freeSTACK(s);
 *  Description: This method frees the stack by freeing the underlying
 *  data structure and then freeing the stack object itself.
 */
void freeSTACK(STACK *items) {
    assert(items != 0);
    freeDLL(items->store);
    free(items);
}
