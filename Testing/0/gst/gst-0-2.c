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
    srandom(43);
    //simple STRING test of GST
    GST *p = newGST(displaySTRING,compareSTRING,freeSTRING);
    insertGST(p,newSTRING("a3"));
    insertGST(p,newSTRING("a2"));
    insertGST(p,newSTRING("a1"));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    STRING *q = newSTRING("a2");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    STRING *r = newSTRING("a3");
    freeSTRING(deleteGST(p,r));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    setSTRING(r,"a2");
    freeSTRING(deleteGST(p,r));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeSTRING(q);
    freeSTRING(r);
    freeGST(p);
    return 0;
    }
