#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "integer.h"
#include "real.h"
#include "string.h"
#include "heap.h"
#include "scanner.h"

/* options */
int vOption = 0;    /* option -v            */
int iOption = 1;    /* option -i (default)  */
int rOption = 0;    /* option -r            */
int sOption = 0;    /* option -s            */
int IOption = 1;    /* option -I (default)  */
int DOption = 0;    /* option -D            */
char *filename;

static int processOptions(int, char **);
void Fatal(char *,...);
void printNameAndExplanation(void);
void readIntegerFileToHeap(HEAP *h, FILE *fp);
void readRealFileToHeap(HEAP *h, FILE *fp);
void readStringFileToHeap(HEAP *h, FILE *fp);
void displaySortedHeap(HEAP *h,
                       void (*display)(void *, FILE *),
                       void (*freeValue)(void *),
                       FILE *fp);

int main(int argc,char **argv) {
    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);

    if (vOption) {
        printNameAndExplanation();
        return 0;
    }

    if (argIndex == argc) {
        Fatal("Missing file name!\n");
    }

    // Open data file for reading
    filename = argv[argIndex];
    FILE *fp = fopen(filename, "r");
    if (fp == 0) Fatal("Unable to open %s for reading!\n", filename);

    HEAP *h;
    if (rOption) {
        // Sort reals
        if (DOption) {
            // Sort in decreasing order
            h = newHEAP(displayREAL, rcompareREAL, freeREAL);
            readRealFileToHeap(h, fp);
            buildHEAP(h);
            displaySortedHeap(h, displayREAL, freeREAL, stdout);
        }
        else {
            // Sort in increasing order
            h = newHEAP(displayREAL, compareREAL, freeREAL);
            readRealFileToHeap(h, fp);
            buildHEAP(h);
            displaySortedHeap(h, displayREAL, freeREAL, stdout);
        }
    }
    else if (sOption) {
        // Sort strings
        if (DOption) {
            // Sort in decreasing order
            h = newHEAP(displaySTRING, rcompareSTRING, freeSTRING);
            readStringFileToHeap(h, fp);
            buildHEAP(h);
            displaySortedHeap(h, displaySTRING, freeSTRING, stdout);
        }
        else {
            // Sort in increasing order
            h = newHEAP(displaySTRING, compareSTRING, freeSTRING);
            readStringFileToHeap(h, fp);
            buildHEAP(h);
            displaySortedHeap(h, displaySTRING, freeSTRING, stdout);
        }
    }
    else {
        // Sort integers
        if (DOption) {
            // Sort in decreasing order
            h = newHEAP(displayINTEGER, rcompareINTEGER, freeINTEGER);
            readIntegerFileToHeap(h, fp);
            buildHEAP(h);
            displaySortedHeap(h, displayINTEGER, freeINTEGER, stdout);
        }
        else {
            // Sort in increasing order
            h = newHEAP(displayINTEGER, compareINTEGER, freeINTEGER);
            readIntegerFileToHeap(h, fp);
            buildHEAP(h);
            displaySortedHeap(h, displayINTEGER, freeINTEGER, stdout);
        }
    }

    fclose(fp);
    freeHEAP(h);
    return 0;
}

void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}

/* only -oXXX  or -o XXX options */

static int processOptions(int argc, char **argv) {
    int argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-') {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        switch (argv[argIndex][1]) {
            /*
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

            case 'v':
                vOption = 1;
                break;
            case 'i':
                iOption = 1;
                break;
            case 'r':
                rOption = 1;
                break;
            case 's':
                sOption = 1;
                break;
            case 'I':
                IOption = 1;
                break;
            case 'D':
                DOption = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }
        ++argIndex;
    }

    return argIndex;
}


void printNameAndExplanation(void) {
    printf("Written by Brett Heithold\n");
    printf("The heapsort algorithm runs in nlogn time. Inserting a value into"
            "the heap takes constant time. Constant time insertion is accomplished"
            "by using a queue and stack to keep track of values as they are"
            "inserted. Every new item added into the unordered heap is inserted"
            "at the leftmost open spot in the bottom level of the heap. Building"
            "the heap takes linear time. Build heap accomplishes linear time"
            "by visiting each of the leaf nodes and running heapify on their parent"
            "nodes, then their grandparents nodes, continuing until the root"
            "node is reached. The extract heap operation takes logn time. The"
            "logarithmic run time of extract heap is accomplished by first"
            "swapping the root's value with the rightmost value on the bottom"
            "level of the heap. Then extract heap prunes the leaf node and"
            "heapifies the root node of the tree.\n");
}


void readIntegerFileToHeap(HEAP *h, FILE *fp) {
    int integer = readInt(fp);
    while (!feof(fp)) {
        insertHEAP(h, newINTEGER(integer));
        integer = readInt(fp);
    }
}


void readRealFileToHeap(HEAP *h, FILE *fp) {
    double real = readReal(fp);
    while (!feof(fp)) {
        insertHEAP(h, newREAL(real));
        real = readReal(fp);
    }
}


void readStringFileToHeap(HEAP *h, FILE *fp) {
    char *str;
    if (stringPending(fp)) str = readString(fp);
    else str = readToken(fp);
    while (!feof(fp)) {
        insertHEAP(h, newSTRING(str));
        if (stringPending(fp)) str = readString(fp);
        else str = readToken(fp);
    }
}


void displaySortedHeap(HEAP *h,
                       void (*display)(void *, FILE *),
                       void (*freeValue)(void *),
                       FILE *fp) {
    void *extracted;
    while (sizeHEAP(h) > 0) {
        extracted = extractHEAP(h);
        display(extracted, fp);
        freeValue(extracted);
        if (sizeHEAP(h) > 0) fprintf(fp, " ");
    }
    fprintf(fp, "\n");
}
