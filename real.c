/*
 *  File:   real.c
 *  Author: Brett Heithold
 *  Description: This is the implementation file for the real module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "real.h"

struct REAL {
    double value;
};

REAL *newREAL(double x) {
    REAL *p = malloc(sizeof(REAL));
    assert(p != 0);
    p->value = x;
    return p;
}

double getREAL(REAL *p) {
    assert(p != 0);
    return p->value;
}

double setREAL(REAL *p, double v) {
    assert(p != 0);
    double old = p->value;
    p->value = v;
    return old;
}

void displayREAL(void *v, FILE *fp) {
    fprintf(fp, "%f", getREAL((REAL *)v));
}

int compareREAL(void *v, void *w) {
    assert(v != NULL && w != NULL);
    if (getREAL(v) < getREAL(w)) return -1;
    else if (getREAL(v) > getREAL(w)) return 1;
    return 0;
}

int rcompareREAL(void *v, void *w) {
    assert(v != NULL && w != NULL);
    if (getREAL(v) > getREAL(w)) return -1;
    else if (getREAL(v) < getREAL(w)) return 1;
    return 0;
}

void freeREAL(void *v) {
    free((REAL *)v);
}
