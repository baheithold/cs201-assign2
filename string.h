/*
 *  File:   string.h
 *  Author: Brett Heithold
 *  Description: This is the public interface file for the string module.
 */

#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

#include <stdio.h>

typedef struct STRING STRING;

extern STRING *newSTRING(char *);
extern char *getSTRING(STRING *);
extern char *setSTRING(STRING *, char *);
extern int compareSTRING(void *, void *);
extern int rcompareSTRING(void *, void *);
extern void displaySTRING(void *, FILE *);
extern void freeSTRING(void *);

#endif // !__STRING_INCLUDED__
