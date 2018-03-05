/*
 *  File:   stack.h
 *  Author: Brett Heithold
 *  Description: This is the interface for the stack class.
 */

#ifndef __STACK_INCLUDED__
#define __STACK_INCLUDED__

#include <stdio.h>

typedef struct STACK STACK;

extern STACK *newSTACK(void (*d)(void *, FILE *), void (*f)(void *));
extern void push(STACK *items, void *value);
extern void *pop(STACK *items);
extern void *peekSTACK(STACK *items);
extern int sizeSTACK(STACK *items);
extern void displaySTACK(STACK *items, FILE *);
extern void displaySTACKdebug(STACK *items, FILE *);
extern void freeSTACK(STACK *items);

#endif // !__STACK_INCLUDED__
