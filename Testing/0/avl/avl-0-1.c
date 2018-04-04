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
    srandom(102);
    //simple REAL test of AVL
    AVL *p = newAVL(displayREAL,compareREAL,freeREAL);
    insertAVL(p,newREAL(2));
    insertAVL(p,newREAL(3));
    insertAVL(p,newREAL(1));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    REAL *q = newREAL(2);
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    REAL *r = newREAL(3);
    freeREAL(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    setREAL(r,2);
    freeREAL(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displayREAL(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeAVL(p));
    freeREAL(q);
    freeREAL(r);
    freeAVL(p);
    return 0;
    }
