/*
 *  File:   real.h
 *  Author: Brett Heithold
 *  Description: This is the public interface for the real module.
 */

#ifndef __REAL_INCLUDED__
#define __REAL_INCLUDED__

#include <stdio.h>

typedef struct REAL REAL;

extern REAL *newREAL(double);
extern double getREAL(REAL *);
extern double setREAL(REAL*, double v);
extern int compareREAL(void *, void *);
extern int rcompareREAL(void *, void *);
extern void displayREAL(void *, FILE *);
extern void freeREAL(void *);

#endif // !__REAL_INCLUDED__
