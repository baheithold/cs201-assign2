/*
 *  File:   avl.c
 *  Author: Brett Heithold
 *  Decription:
 */


#include "avl.h"
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct aval {
    void *value;
    int count;
    int leftHeight;
    int rightHeight;
    int height;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*free)(void *);
} AVAL;

AVAL *newAVAL(
        void *v,
        void (*d)(void *, FILE *),
        int (*c)(void *, void *),
        void (*f)(void *)) {
    AVAL *rv = malloc(sizeof(AVAL));
    assert(rv != 0);
    rv->value = v;
    rv->count = 1;
    rv->leftHeight = 0;
    rv->rightHeight = 0;
    rv->height = 1;
    rv->display = d;
    rv->compare = c;
    rv->free = f;
    return rv;
}

void *getAVALvalue(AVAL *av) {
    assert(av != 0);
    return av->value;
}

void setAVALvalue(AVAL *av, void *v) {
    av->value = v;
}

int getAVALcount(AVAL *av) {
    assert(av != 0);
    return av->count;
}

void incrementAVALcount(AVAL *av) {
    assert(av != 0);
    av->count++;
}

void decrementAVALcount(AVAL *av) {
    assert(av != 0);
    av->count--;
}

int getAVALheight(AVAL *av) {
    assert(av != 0);
    return av->height;
}

void setAVALheight(AVAL *av, int height) {
    assert(av != 0);
    av->height = height;
}

int getAVALbalance(AVAL *av) {
    assert(av != 0);
    return av->leftHeight - av->rightHeight;
}

void setAVALbalanceFactors(AVAL *av, int lh, int rh) {
    assert(av != 0);
    av->leftHeight = lh;
    av->rightHeight = rh;
    av->height = lh > rh ? lh + 1 : rh + 1;
}

void adisplay(void *v, FILE *fp) {
    assert(v != 0);
    ((AVAL *) v)->display(getAVALvalue((AVAL *)v), fp);
    int count = getAVALcount((AVAL *)v);
    if (count > 1) fprintf(fp, "[%d]", count);
    int bal = getAVALbalance((AVAL *) v);
    if (bal == 1) fprintf(fp, "+");
    else if (bal == -1) fprintf(fp, "-");
}

int compareAVAL(void *v, void *w) {
    return (((AVAL *)v)->compare(getAVALvalue(v), getAVALvalue(w)));
}

void freeAVAL(void *v) {
    assert(v != 0);
    if (((AVAL *)v)->free != NULL) {
        ((AVAL *)v)->free(getAVALvalue(v));
    }
    free(v);
}


/********** Private AVL method prototypes **********/
void insertionFixUp(AVL *, BSTNODE *);
void deletionFixUp(AVL *, BSTNODE *);
void rotateTo(AVL *, BSTNODE *, BSTNODE *);
int isRoot(AVL *, BSTNODE *);
void swapper(BSTNODE *, BSTNODE *);
int height(BSTNODE *);
void setBalance(BSTNODE *);
int getBalance(BSTNODE *);
BSTNODE *sibling(BSTNODE *);
BSTNODE *favoriteChild(BSTNODE *);
int linear(BSTNODE *c, BSTNODE *p, BSTNODE *gp);


struct AVL {
    BST *store;
    int size;
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*free)(void *);
    void (*insertionFixUp)(AVL *, BSTNODE *);
    void (*deletionFixUp)(AVL *, BSTNODE *);
    void (*rotateTo)(AVL *, BSTNODE *, BSTNODE *);
    int (*isRoot)(AVL *, BSTNODE *);
};

AVL *newAVL(
        void (*d)(void *, FILE *),
        int (*c)(void *, void *),
        void (*f)(void *)) {
    AVL *rv = malloc(sizeof(AVL));
    assert(rv != 0);
    rv->store = newBST(adisplay, compareAVAL, swapper, freeAVAL);
    rv->size = 0;
    rv->display = d;
    rv->compare = c;
    rv->free = f;
    rv->insertionFixUp = insertionFixUp;
    rv->deletionFixUp = deletionFixUp;
    rv->rotateTo = rotateTo;
    rv->isRoot = isRoot;
    return rv;
}

void insertAVL(AVL *t, void *v) {
    assert(t != 0);
    AVAL *temp = newAVAL(v, t->display, t->compare, t->free);
    BSTNODE *n = findBST(t->store, temp);
    if (n == NULL) {
        // Tree does not contain value
        n = insertBST(t->store, temp);
        setBalance(n);
        t->insertionFixUp(t, n);
    }
    else {
        // Tree already contains the value
        incrementAVALcount(getBSTNODEvalue(n));
        free((AVAL *) temp);
    }
    t->size++;
}

int findAVLcount(AVL *t, void *v) {
    assert(t != 0);
    AVAL *temp = newAVAL(v, t->display, t->compare, NULL);
    BSTNODE *n = findBST(t->store, temp);
    freeAVAL(temp);
    return n == NULL ? 0 : getAVALcount(getBSTNODEvalue(n));
}

void *findAVL(AVL *t, void *v) {
    assert(t != 0);
    AVAL *temp = newAVAL(v, NULL, t->compare, NULL);
    BSTNODE *n = findBST(t->store, temp);
    freeAVAL(temp);
    return n == NULL ? NULL : getAVALvalue(getBSTNODEvalue(n));
}

void *deleteAVL(AVL *t, void *v) {
    void *rv = NULL;
    AVAL *temp = newAVAL(v, t->display, t->compare, NULL);
    BSTNODE *n = findBST(t->store, temp);
    if (n == NULL) {
        // Value not found in tree
        freeAVAL(temp);
        return NULL;
    }
    else {
        if (getAVALcount(getBSTNODEvalue(n)) > 1) {
            // Value has duplicates
            decrementAVALcount(getBSTNODEvalue(n));
            rv = v;
        }
        else {
            // Value found, no duplicates
            BSTNODE *leaf = swapToLeafBST(t->store, n);
            rv = getAVALvalue(getBSTNODEvalue(leaf));
            BSTNODE *p = getBSTNODEparent(leaf);
            setBalance(leaf);
            t->deletionFixUp(t, leaf);
            pruneLeafBST(t->store, leaf);
            setBalance(p); // FIXME: Am I correct???
            setBSTsize(t->store, sizeBST(t->store) - 1);
            free((AVAL *) getBSTNODEvalue(leaf));
            freeBSTNODE(leaf, NULL);
        }
        t->size--;
    }
    freeAVAL(temp);
    return rv;
}

int sizeAVL(AVL *t) {
    assert(t != 0);
    return sizeBST(t->store);
}

int duplicatesAVL(AVL *t) {
    assert(t != 0);
    return t->size - sizeBST(t->store);
}

void statisticsAVL(AVL *t, FILE *fp) {
    assert(t != 0);
    fprintf(fp, "Duplicates: %d\n", duplicatesAVL(t));
    statisticsBST(t->store, fp);
}

void displayAVL(AVL *t, FILE *fp) {
    assert(t != 0);
    displayBSTdecorated(t->store, fp);
}

void displayAVLdebug(AVL *t, FILE *fp) {
    assert(t != 0);
    displayBST(t->store, fp);
}

void freeAVL(AVL *t) {
    assert(t != 0);
    freeBST(t->store);
    free(t);
}


/*************************** Private methods ***************************/

void insertionFixUp(AVL *t, BSTNODE *x) {
    while (!t->isRoot(t, x)) {
        if (sibling(x) && sibling(x) == favoriteChild(getBSTNODEparent(x))) {
            // Parent favors sibling
            BSTNODE *p = getBSTNODEparent(x);
            setBalance(p);
            break;
        }
        else if (favoriteChild(getBSTNODEparent(x)) == NULL) {
            // Parent is balanced
            BSTNODE *p = getBSTNODEparent(x);
            setBalance(p);
            x = p;
        }
        else {
            BSTNODE *y = favoriteChild(x);
            BSTNODE *p = getBSTNODEparent(x);
            if (y && !linear(y, x, p)) {
                t->rotateTo(t, y, x);
                t->rotateTo(t, y, p);
                // set balance of x
                setBalance(x);
                // set balance of p
                setBalance(p);
                // set balance of y
                setBalance(y);
            }
            else {
                t->rotateTo(t, x, p);
                // set balance of p
                setBalance(p);
                // set balance of x
                setBalance(x);
            }
            break;
        }
    }
}

void deletionFixUp(AVL *t, BSTNODE *x) {
    setAVALheight(getBSTNODEvalue(x), 0);
    while (!t->isRoot(t, x)) {
        BSTNODE *p = getBSTNODEparent(x);
        if (x == favoriteChild(p)) {
            // x is favored by parent
            setBalance(p);
            x = p;
        }
        else if (favoriteChild(p) == NULL) {
            // Parent is balanced, has no favorite
            setBalance(p);
            break;
        }
        else {
            p = getBSTNODEparent(x);
            BSTNODE *z = sibling(x);
            BSTNODE *y = favoriteChild(z);
            if (y && !linear(y, z, p)) {
                t->rotateTo(t, y, z);
                t->rotateTo(t, y, p);
                // set balance p
                setBalance(p);
                // set balance z
                setBalance(z);
                // set balance y
                setBalance(y);
                x = y;
            }
            else {
                t->rotateTo(t, z, p);
                // set balance p
                setBalance(p);
                // set balance z
                setBalance(z);
                if (!y) {
                    break;
                }
                x = z;
            }
        }

    }
}

void rotateTo(AVL *t, BSTNODE *y, BSTNODE *x) {
    int rootNeedsUpdating = 0;
    if (t->isRoot(t, x)) rootNeedsUpdating = 1;
    if (y == getBSTNODEleft(x)) {
        // Right rotation
        setBSTNODEleft(x, getBSTNODEright(y));
        if (getBSTNODEright(y) != NULL) {
            setBSTNODEparent(getBSTNODEright(y), x);
        }
        setBSTNODEparent(y, getBSTNODEparent(x));
        if (rootNeedsUpdating) {
            setBSTNODEparent(y, y);
            setBSTroot(t->store, y);
        }
        else if (x == getBSTNODEright(getBSTNODEparent(x))) {
            setBSTNODEright(getBSTNODEparent(x), y);
        }
        else {
            setBSTNODEleft(getBSTNODEparent(x), y);
        }
        setBSTNODEright(y, x);
        setBSTNODEparent(x, y);
    }
    else {
        // Left rotation
        setBSTNODEright(x, getBSTNODEleft(y));
        if (getBSTNODEleft(y) != NULL) {
            setBSTNODEparent(getBSTNODEleft(y), x);
        }
        setBSTNODEparent(y, getBSTNODEparent(x));
        if (rootNeedsUpdating) {
            setBSTNODEparent(y, y);
            setBSTroot(t->store, y);
        }
        else if (x == getBSTNODEleft(getBSTNODEparent(x))) {
            setBSTNODEleft(getBSTNODEparent(x), y);
        }
        else {
            setBSTNODEright(getBSTNODEparent(x), y);
        }
        setBSTNODEleft(y, x);
        setBSTNODEparent(x, y);
    }
}

int isRoot(AVL *t, BSTNODE *n) {
    assert(t != 0);
    return n == getBSTroot(t->store) ? 1 : 0;
}

void swapper(BSTNODE *a, BSTNODE *b) {
    AVAL *ta = getBSTNODEvalue(a);
    AVAL *tb = getBSTNODEvalue(b);
    void *vtemp = ta->value;
    ta->value = tb->value;
    tb->value = vtemp;
    int ctemp = ta->count;
    ta->count = tb->count;
    tb->count = ctemp;
}

int height(BSTNODE *n) {
    if (n == NULL) return 0;
    BSTNODE *leftChild = getBSTNODEleft(n);
    BSTNODE *rightChild = getBSTNODEright(n);
    int lh = 0;
    int rh = 0;
    if (leftChild) lh = ((AVAL *)getBSTNODEvalue(leftChild))->height;
    if (rightChild) rh = ((AVAL *)getBSTNODEvalue(rightChild))->height;
    return lh > rh ? lh + 1 : rh + 1;
}

int getBalance(BSTNODE *n) {
    assert(n != 0);
    return getAVALbalance(getBSTNODEvalue(n));
}

void setBalance(BSTNODE *n) {
    assert(n != 0);
    BSTNODE *leftChild = getBSTNODEleft(n);
    BSTNODE *rightChild = getBSTNODEright(n);
    AVAL *av = getBSTNODEvalue(n);
    setAVALbalanceFactors(av, height(leftChild), height(rightChild));
    int lh = 0;
    int rh = 0;
    if (leftChild) {
        lh = getAVALheight(getBSTNODEvalue(leftChild));
    }
    if (rightChild) {
        rh = getAVALheight(getBSTNODEvalue(rightChild));
    }
    setAVALheight(av, lh > rh ? lh + 1 : rh + 1);
}

BSTNODE *sibling(BSTNODE *c) {
    assert(c != 0);
    BSTNODE *parent = getBSTNODEparent(c);
    if (parent == c) return NULL; // Root
    else if (getBSTNODEleft(parent) == c) return getBSTNODEright(parent);
    else return getBSTNODEleft(parent);
}

BSTNODE *favoriteChild(BSTNODE *p) {
    assert(p != 0);
    if (getBalance(p) == 1) {
        // Left child is favorite
        return getBSTNODEleft(p);
    }
    else if (getBalance(p) == -1) {
        // Right child is favorite
        return getBSTNODEright(p);
    }
    // Balance == 0, neither child is favored
    return NULL;
}

int linear(BSTNODE *c, BSTNODE *p, BSTNODE *gp) {
    assert(c != NULL);
    assert(p != NULL);
    assert(gp != NULL);
    int leftLinear = getBSTNODEleft(gp) == p && getBSTNODEleft(p) == c;
    int rightLinear = getBSTNODEright(gp) == p && getBSTNODEright(p) == c;
    return leftLinear || rightLinear;
}
