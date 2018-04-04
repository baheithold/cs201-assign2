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
    srandom(103);
    //simple STRING test of AVL
    AVL *p = newAVL(displaySTRING,compareSTRING,freeSTRING);
    insertAVL(p,newSTRING("a2"));
    insertAVL(p,newSTRING("a3"));
    insertAVL(p,newSTRING("a1"));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    STRING *q = newSTRING("a2");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    STRING *r = newSTRING("a3");
    freeSTRING(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    setSTRING(r,"a2");
    freeSTRING(deleteAVL(p,r));
    statisticsAVL(p,stdout);
    printf("AVL:\n");
    displayAVL(p,stdout);
    printf("debug: ");
    displayAVLdebug(p,stdout);
    printf("\n");
    printf("is ");
    displaySTRING(q,stdout);
    printf(" present? %s\n",findAVL(p,q) == 0? "no" : "yes");
    printf("size: %d\n",sizeAVL(p));
    freeSTRING(q);
    freeSTRING(r);
    freeAVL(p);
    return 0;
    }
