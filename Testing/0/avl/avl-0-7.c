#include <stdio.h>
#include <stdlib.h>
#include "../../../integer.h"
#include "../../../real.h"
#include "../../../string.h"
#include "../../../queue.h"
#include "../../../bst.h"
#include "../../../avl.h"

void srandom(unsigned int);
long int random(void);

int
main(void)
    {
    srandom(108);
    //INTEGER test of AVL, nominal size is 10000
    int i;
    AVL *p = newAVL(displayINTEGER,compareINTEGER,freeINTEGER);
    for (i = 0; i < 10000; ++i)
        {
        int j = random() % 10000;
        INTEGER *a = newINTEGER(j);
        insertAVL(p,a);
        if (findAVLcount(p,a) > 1)
            freeINTEGER(a);
        }
    if (sizeAVL(p) < 200)
        {
        printf("AVL:\n");
        displayAVL(p,stdout);
        printf("debug: ");
        displayAVLdebug(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeAVL(p));
    while (sizeAVL(p) > 200)
        {
        for (i = 0; i < 10000; ++i)
            {
            int j = random() % 10000;
            INTEGER *a = newINTEGER(j);
            int count = findAVLcount(p,a);
            if (count == 1)
                freeINTEGER(deleteAVL(p,a));
            else if (count > 1)
                deleteAVL(p,a);
            freeINTEGER(a);
            }
        }
    if (sizeAVL(p) < 200)
        {
        printf("AVL:\n");
        displayAVL(p,stdout);
        printf("debug: ");
        displayAVLdebug(p,stdout);
        printf("\n");
        }
    printf("size is %d\n",sizeAVL(p));
    statisticsAVL(p,stdout);
    freeAVL(p);
    return 0;
    }
