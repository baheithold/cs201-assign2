/*
 *  File:   avl.h
 *  Author: Brett Heithold
 *  Description:
 */

#ifndef __AVL_INCLUDED__
#define __AVL_INCLUDED__

#include <stdio.h>

typedef struct AVL AVL;

extern AVL *newAVL(
        void (*)(void *, FILE *),
        int (*)(void *, void *),
        void (*)(void *));
extern void insertAVL(AVL *, void *);
extern int findAVLcount(AVL *, void *);
extern void *findAVL(AVL *, void *);
extern void *deleteAVL(AVL *, void *);
extern int sizeAVL(AVL *);
extern int duplicatesAVL(AVL *);
extern void statisticsAVL(AVL *, FILE *);
extern void displayAVL(AVL *, FILE *);
extern void displayAVLdebug(AVL *, FILE *);
extern void freeAVL(AVL *);

#endif // !__AVL_INCLUDED__
