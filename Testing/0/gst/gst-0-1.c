#include <stdio.h>
#include <stdlib.h>
#include "../../../bst.h"
#include "../../../gst.h"
#include "../../../integer.h"
#include "../../../real.h"
#include "../../../string.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(42);
    //simple REAL test of GST
    GST *p = newGST(displayREAL,compareREAL,freeREAL);
    insertGST(p,newREAL(3));
    insertGST(p,newREAL(2));
    insertGST(p,newREAL(1));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    REAL *q = newREAL(2);
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    REAL *r = newREAL(3);
    freeREAL(deleteGST(p,r));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    setREAL(r,2);
    freeREAL(deleteGST(p,r));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeREAL(q);
    freeREAL(r);
    freeGST(p);
    return 0;
    }
