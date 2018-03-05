/*
 * File:    dll.h
 * Author:  Brett Heithold
 * Description: Interface file for the doubly-linked list class.
 */

#ifndef __DLL_INCLUDED__
#define __DLL_INCLUDED__

#include <stdio.h>

typedef struct DLL DLL;

extern DLL *newDLL(void (*d)(void *, FILE *), void (*f)(void *));
extern void insertDLL(DLL *items, int index, void *value);
extern void *removeDLL(DLL *items, int index);
extern void unionDLL(DLL *recipient, DLL *donor);
extern void *getDLL(DLL *items, int index);
extern void *setDLL(DLL *items, int index, void *value);
extern int sizeDLL(DLL *items);
extern void displayDLL(DLL *items, FILE *);
extern void displayDLLdebug(DLL *items, FILE *);
extern void freeDLL(DLL *items);

#endif //!__DLL_INCLUDED__
