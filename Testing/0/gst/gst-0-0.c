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
    srandom(41);
    //simple INTEGER test of GST
    GST *p = newGST(displayINTEGER,compareINTEGER,freeINTEGER);
    insertGST(p,newINTEGER(3));
    insertGST(p,newINTEGER(2));
    insertGST(p,newINTEGER(1));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    INTEGER *q = newINTEGER(2);
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    INTEGER *r = newINTEGER(3);
    freeINTEGER(deleteGST(p,r));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    setINTEGER(r,2);
    freeINTEGER(deleteGST(p,r));
    printf("GST:\n");
    displayGST(p,stdout);
    printf("debug: ");
    displayGSTdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayINTEGER(q,stdout);
    printf(" present? %s\n",findGST(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeINTEGER(q);
    freeINTEGER(r);
    freeGST(p);
    return 0;
    }
