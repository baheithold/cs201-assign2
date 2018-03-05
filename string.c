/*
 *  File:   string.c
 *  Author: Brett Heithold
 *  Description: This is the implementation file for the string module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "string.h"

struct STRING {
    char *value;
};

STRING *newSTRING(char *x) {
    STRING *p = malloc(sizeof(STRING));
    assert(p != 0);
    p->value = x;
    return p;
}

char *getSTRING(STRING *p) {
    assert(p != 0);
    return p->value;
}

char *setSTRING(STRING *p, char *v) {
    assert(p != 0);
    char *old = p->value;
    p->value = v;
    return old;
}

int compareSTRING(void *str1, void *str2) {
    assert(str1 != NULL && str2 != NULL);
    return strcmp(getSTRING(str1), getSTRING(str2));
}

int rcompareSTRING(void *str1, void *str2) {
    assert(str1 != NULL && str2 != NULL);
    return strcmp(getSTRING(str2), getSTRING(str1));
}

void displaySTRING(void *v, FILE *fp) {
    fprintf(fp, "%s", getSTRING((STRING *)v));
}

void freeSTRING(void *v) {
    free((STRING *)v);
}
