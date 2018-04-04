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
    srandom(46);
    //simple INTEGER test of GST
    int i;
    GST *p = newGST(displayINTEGER,compareINTEGER,freeINTEGER);
    for (i = 0; i < 189; ++i)
        {
        int j = random() % 189;
        INTEGER *a = newINTEGER(j);
        if (findGST(p,a) == 0)
            insertGST(p,a);
        else
            freeINTEGER(a);
        }
    if (sizeGST(p) < 200)
        {
        printf("GST:\n");
        displayGST(p,stdout);
        printf("debug: ");
        displayGSTdebug(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeGST(p));
    statisticsGST(p,stdout);
    freeGST(p);
    return 0;
    }
