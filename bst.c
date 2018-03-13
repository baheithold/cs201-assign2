/*
 *  File:   bst.c
 *  Author: Brett Heithold
 *  Description: This is the implementation file for the bst(binary search tree)
 *  module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "queue.h"


// BSTNODE private method prototypes
static int isLeaf(BSTNODE *n);


/*
 *  Type:   BSTNODE
 *  Description: This is the node structure to be used in the BST class.
 */
struct BSTNODE {
    void *value;
    BSTNODE *left;
    BSTNODE *right;
    BSTNODE *parent;
    int (*isLeaf)(BSTNODE *);
};


/*
 *  Constructor: newBSTNODE
 *  Usage:  BSTNODE *n = newBSTNODE(value);
 *  Description: This is the constructor used to instantiate a new
 *  BSTNODE object.
 */
BSTNODE *newBSTNODE(void *v) {
    BSTNODE *n = malloc(sizeof(BSTNODE));
    assert(n != 0);
    n->value = v;
    n->left = NULL;
    n->right = NULL;
    n->parent = NULL;
    n->isLeaf = isLeaf;
    return n;
}


/*
 *  Method: getBSTNODEvalue
 *  Usage:  void *v = getBSTNODEvalue(n);
 *  Description: This method returns the value held by a BSTNODE object.
 */
void *getBSTNODEvalue(BSTNODE *n) {
    assert(n != 0);
    return n->value;
}


/*
 *  Method: setBSTNODEvalue
 *  Usage:  setBSTNODEvalue(n, v);
 *  Description: This method sets the value held by a BSTNODE object.
 */
void setBSTNODEvalue(BSTNODE *n, void *value) {
    assert(n != 0);
    n->value = value;
}


/*
 *  Method: getBSTNODEleft
 *  Usage:  BSTNODE *left = getBSTNODEleft(n);
 *  Description: This method returns the left pointer of a BSTNODE.
 */
BSTNODE *getBSTNODEleft(BSTNODE *n) {
    assert(n != 0);
    return n->left;
}


/*
 *  Method: setBSTNODEleft
 *  Usage:  setBSTNODEleft(n);
 *  Description: This method sets the left pointer of a BSTNODE.
 */
void setBSTNODEleft(BSTNODE *n, BSTNODE *replacement) {
    assert(n != 0);
    n->left = replacement;
}


/*
 *  Method: getBSTNODEright
 *  Usage:  BSTNODE *right = getBSTNODEright(n);
 *  Description: This method returns the right pointer of a BSTNODE.
 */
BSTNODE *getBSTNODEright(BSTNODE *n) {
    assert(n != 0);
    return n->right;
}


/*
 *  Method: setBSTNODEright
 *  Usage:  setBSTNODEright(n);
 *  Description: This method sets the right pointer of a BSTNODE.
 */
void setBSTNODEright(BSTNODE *n, BSTNODE *replacement) {
    assert(n != 0);
    n->right = replacement;
}


/*
 *  Method: getBSTNODEparent
 *  Usage:  BSTNODE *parent = getBSTNODEparent(n);
 *  Description: This method returns the parent pointer of a BSTNODE.
 */
BSTNODE *getBSTNODEparent(BSTNODE *n) {
    assert(n != 0);
    return n->parent;
}


/*
 *  Method: setBSTNODEparent
 *  Usage:  setBSTNODEparent(n);
 *  Description: This method sets the parent pointer of a BSTNODE.
 */
void setBSTNODEparent(BSTNODE *n, BSTNODE *replacement) {
    assert(n != 0);
    n->parent = replacement;
}


/*
 *  Method: freeBSTNODE
 *  Usage:  freeBSTNODE(n, freeINTEGER);
 *  Description: If the freeing function is not NULL, then the method should
 *  free its generic value before freeing the node itself.
 */
void freeBSTNODE(BSTNODE *n, void (*freeValue)(void *)) {
    assert(n != 0);
    if (freeValue != NULL) {
        freeValue(n->value);
    }
    free(n);
}


/*
 *  Method (private): isLeaf
 *  Usage:  int isNodeLeaf = isLeaf(n);
 *  Description: This is private method returns true if the node is a leaf node
 *  and false otherwise.
 */
int isLeaf(BSTNODE *n) {
    assert(n != 0);
    if (getBSTNODEleft(n) != NULL || getBSTNODEright(n) != NULL) {
        return 0;
    }
    return 1;
}


// BST private method prototypes
static void swapper(BSTNODE *x, BSTNODE *y);
static int isRoot(BST *t, BSTNODE *n);
static int isLeftChild(BST *t, BSTNODE *n);
static int isRightChild(BST *t, BSTNODE *n);
static int getMinDepth(BST *t, BSTNODE *n);
static int getMaxDepth(BST *t, BSTNODE *n);
static BSTNODE *getTreeMinimum(BSTNODE *n);
static BSTNODE *getTreeMaximum(BSTNODE *n);
static BSTNODE *getSuccessor(BST *t, BSTNODE *n);
static BSTNODE *getPredecessor(BST *t, BSTNODE *n);
static void displayPreorder(BST *t, BSTNODE *n, FILE *fp);
static void freeTree(BST *t, BSTNODE *n);


/*
 *  Type:   BST
 *  Description: This is the struct definition for the BST class.
 */
struct BST {
    BSTNODE *root;
    int size;

    // Public Methods
    void (*display)(void *, FILE *);
    int (*compare)(void *, void *);
    void (*swap)(BSTNODE *, BSTNODE *);
    void (*free)(void *);

    // Private Methods
    int (*isRoot)(BST *, BSTNODE *);
    int (*isLeftChild)(BST *, BSTNODE *);
    int (*isRightChild)(BST *, BSTNODE *);
    int (*getMinDepth)(BST *, BSTNODE *);
    int (*getMaxDepth)(BST *, BSTNODE *);
    BSTNODE *(*getTreeMinimum)(BSTNODE *);
    BSTNODE *(*getTreeMaximum)(BSTNODE *);
    BSTNODE *(*getSuccessor)(BST *, BSTNODE *);
    BSTNODE *(*getPredecessor)(BST *, BSTNODE *);
    void (*displayPreorder)(BST *, BSTNODE *, FILE *);
    void (*freeTree)(BST *, BSTNODE *);
};


/*
 *  Constructor: newBST
 *  Usage:  BST *t = newBST(displayINT, compareINT, swapINT, freeINT);
 *  Description: This is the constructor used to instantiate a new
 *  BST object.
 */
BST *newBST(void (*d)(void *, FILE *),
            int (*c)(void *, void *),
            void (*s)(BSTNODE *, BSTNODE *),
            void (*f)(void *)) {
    // TODO: Fix swapper handling
    BST *t = malloc(sizeof(BST));
    assert(t != 0);
    t->root = NULL;
    t->size = 0;
    t->display = d;
    t->compare = c;
    if (s != NULL) t->swap = s;
    else t->swap = swapper;
    t->free = f;
    t->isRoot = isRoot;
    t->isLeftChild = isLeftChild;
    t->isRightChild = isRightChild;
    t->getMinDepth = getMinDepth;
    t->getMaxDepth = getMaxDepth;
    t->getTreeMinimum = getTreeMinimum;
    t->getTreeMaximum = getTreeMaximum;
    t->getSuccessor = getSuccessor;
    t->getPredecessor = getPredecessor;
    t->displayPreorder = displayPreorder;
    t->freeTree = freeTree;
    return t;
}


/*
 *  Method: getBSTroot
 *  Usage:  BSTNODE *root = getBSTroot(t);
 *  Description: This method returns the root NODE of a tree.
 */
BSTNODE *getBSTroot(BST *t) {
    assert(t != NULL);
    return t->root;
}


/*
 *  Method: setBSTroot
 *  Usage:  setBSTroot(t, replacement);
 *  Description: This method updates the root pointer of a BST object. This
 *  method runs in constant time.
 */
void setBSTroot(BST *t, BSTNODE *replacement) {
    assert(t != 0);
    t->root = replacement;
}


/*
 *  Method: setBSTsize
 *  Usage:  setBSTsize(t, size);
 *  Description: This method sets the size of a BST object.
 */
void setBSTsize(BST *t, int s) {
    assert(t != 0);
    t->size = s;
}


/*
 *  Method: insertBST
 *  Usage:  BSTNODE *n = insertBST(tree, value);
 *  Description: This method inserts a new value into a tree.
 */
BSTNODE *insertBST(BST *t, void *value) {
    assert(t != 0);
    BSTNODE *n = newBSTNODE(value);
    assert(n != 0);
    BSTNODE *p = NULL;
    BSTNODE *x = t->root;
    while (x != NULL) {
        p = x;
        if (t->compare(value, getBSTNODEvalue(x)) < 0) {
            // Traverse Left
            x = getBSTNODEleft(x);
        }
        else {
            // Traverse Right
            x = getBSTNODEright(x);
        }
    }
    setBSTNODEparent(n, p);
    if (p == NULL) {
        setBSTroot(t, n);
    }
    else if (t->compare(value, getBSTNODEvalue(p)) < 0) {
        // Set the new node to be the left child of p
        setBSTNODEleft(p, n);
    }
    else {
        // Set the new node to be the right child of p
        setBSTNODEright(p, n);
    }
    t->size++;
    return n;
}


/*
 *  Method: findBST
 *  Usage:  BSTNODE *n = findBST(tree, 7);
 *  Description: This method returns the node that holds the searched-for value.
 *  If the value is not in the tree, the method returns null. This method
 *  runs in logarithmic time.
 */
BSTNODE *findBST(BST *t, void *value) {
    assert(t != 0);
    BSTNODE *n = t->root;
    while (n != NULL && t->compare(value, getBSTNODEvalue(n)) != 0) {
        if (t->compare(value, getBSTNODEvalue(n)) < 0) {
            // Traverse Left
            n = getBSTNODEleft(n);
        }
        else {
            // Traverse Right
            n = getBSTNODEright(n);
        }
    }
    return n;
}


/*
 *  Method: deleteBST
 *  Usage:  BSTNODE *deletedNode = deleteBST(t, valueToDelete);
 *  Description:
 */
BSTNODE *deleteBST(BST *t, void *v) {
    BSTNODE *n = findBST(t, v);
    if (n == NULL) return NULL;
    n = swapToLeafBST(t, n);
    pruneLeafBST(t, n);
    t->size--;
    return n;
}


/*
 *  Method: swapToLeaf
 *  Usage:  BSTNODE *leaf = swapToLeaf(t, node);
 *  Description:
 */
BSTNODE *swapToLeafBST(BST *t, BSTNODE *n) {
    // TODO: Am I correct?
    assert(t != 0 && n != 0);
    BSTNODE *leaf = n;
    while (n->isLeaf(n) != 1) {
        if (getBSTNODEright(n) != NULL) {
            leaf = t->getSuccessor(t, n);
            t->swap(n, leaf);
        }
        else {
            leaf = t->getPredecessor(t, n);
            t->swap(n, leaf);
        }
        n = leaf;
    }
    return leaf;
}


/*
 *  Method: pruneLeafBST
 *  Usage:  pruneLeafBST(t, n);
 *  Description:
 */
void pruneLeafBST(BST *t, BSTNODE *leaf) {
    assert(t != 0);
    if (t->size == 1) {
        setBSTroot(t, NULL);
    }
    else if (t->isLeftChild(t, leaf)) {
        setBSTNODEleft(getBSTNODEparent(leaf), NULL);
        setBSTNODEparent(leaf, NULL);
    }
    else {
        setBSTNODEright(getBSTNODEparent(leaf), NULL);
        setBSTNODEparent(leaf, NULL);
    }
}


/*
 *  Method: sizeBST
 *  Usage:  int s = sizeBST(t);
 *  Description: This method returns the number of nodes in a BST object.
 */
int sizeBST(BST *t) {
    assert(t != 0);
    return t->size;
}


/*
 *  Method: statisticesBST
 *  Usage:  statisticsBST(t, stdout);
 *  Description: This method displays the number of nodes in the tree as well
 *  as the minimum and maximum heights of the tree. The minimum depth of a tree
 *  is the minimum number of steps from the root to a node with a NULL child.
 *  The maximum depth of a tree is the maximum number of steps from the root
 *  to a node with a NULL child. The depths of an empty tree are -1.
 *  This method runs in linear time.
 *  Example Output:
 *                  Nodes: 8
 *                  Minimum depth: 2
 *                  Maximum depth: 4
 */
void statisticsBST(BST *t, FILE *fp) {
    assert(t != 0);
    fprintf(fp, "Nodes: %d\n", t->size);
    int minDepth = -1;
    int maxDepth = -1;
    if (t->size > 0) {
        minDepth = t->getMinDepth(t, t->root);
        maxDepth = t->getMaxDepth(t, t->root);
    }
    fprintf(fp, "Minimum depth: %d\n", minDepth);
    fprintf(fp, "Maximum depth: %d\n", maxDepth);
}


/*
 *  Method: displayBST
 *  Usage:  displayBST(t, stdout);
 *  Description:
 *  Example Output:
 */
void displayBST(BST *t, FILE *fp) {
    fprintf(fp, "[");
    t->displayPreorder(t, t->root, fp);
    fprintf(fp, "]");
}


/*
 *  Method: displayBSTdebug
 *  Usage:  displayBSTdebug(t, stdout);
 *  Description: This method displays a level-order traversal of a binary tree
 *  object. Each level is displayed on a new line with spaces separating
 *  each node. This method runs in linear time.
 *  Example Output:
 *                  7
 *                  4 10
 *                  2 5 9 12
 *                  8
 */
void displayBSTdebug(BST *t, FILE *fp) {
    assert(t != 0);
    if (t->root == NULL) return;
    QUEUE *q = newQUEUE(NULL, NULL);
    BSTNODE *n = t->root;
    enqueue(q, n);
    int nodesAtLevel = 0;
    while (1) {
        nodesAtLevel = sizeQUEUE(q);
        if (nodesAtLevel == 0) break;
        while (nodesAtLevel > 0) {
            n = dequeue(q);
            t->display(getBSTNODEvalue(n), fp);
            if (nodesAtLevel > 1) fprintf(fp, " ");
            if (n->left != NULL) enqueue(q, n->left);
            if (n->right != NULL) enqueue(q, n->right);
            nodesAtLevel--;
        }
        fprintf(fp, "\n");
    }
    freeQUEUE(q);
}


/*
 *  Method: displayBSTdecorated
 *  Usage:  displayBSTdecorated(t, stdout);
 *  Description: TODO
 *  Example Output: TODO
 */
void displayBSTdecorated(BST *t, FILE *fp) {
    // TODO: Am I correct?
    // TODO: Am I efficient?
    assert(t != 0);
    if (t->root == NULL) return;
    QUEUE *q = newQUEUE(NULL, NULL);
    BSTNODE *n = t->root;
    enqueue(q, n);
    int level = 0;
    int nodesAtLevel = 0;
    while (1) {
        nodesAtLevel = sizeQUEUE(q);
        if (nodesAtLevel == 0) break;
        fprintf(fp, "%d: ", level);
        while (nodesAtLevel > 0) {
            n = dequeue(q);
            if (n->isLeaf(n)) fprintf(fp, "=");
            t->display(getBSTNODEvalue(n), fp);
            if (t->isRoot(t, n)) {
                fprintf(fp, "(");
                t->display(getBSTNODEvalue(n), fp);
                fprintf(fp, ")");
            }
            else {
                fprintf(fp, "(");
                t->display(getBSTNODEvalue(n->parent), fp);
                fprintf(fp, ")");
            }
            if (t->isRoot(t, n)) {
                fprintf(fp, "X");
            }
            else if (t->isLeftChild(t, n)) fprintf(fp, "L");
            else if (t->isRightChild(t, n)) fprintf(fp, "R");
            if (nodesAtLevel > 1) fprintf(fp, " ");
            if (n->left != NULL) enqueue(q, n->left);
            if (n->right != NULL) enqueue(q, n->right);
            nodesAtLevel--;
        }
        fprintf(fp, "\n");
        level++;
    }
    freeQUEUE(q);
}


/*
 *  Method: freeBST
 *  Usage:  freeBST(tree);
 *  Description: This method frees a BST object by performing a postorder
 *  traversal of the tree freeing the BSTNODE objects. After freeing all of the
 *  BSTNODEs, the tree object itself is freed. This method makes use of a
 *  recursive helper function.
 */
void freeBST(BST *t) {
    t->freeTree(t, t->root);
    free(t);
}


/****************************** Private Methods ******************************/


/*
 *  Method (private):   swapper
 *  Usage:  t->swapper(x, y);
 *  Description:
 */
void swapper(BSTNODE *x, BSTNODE *y) {
    // TODO: Do I work correctly?
    assert(x != 0 && y != 0);
    void *tmp = getBSTNODEvalue(x);
    setBSTNODEvalue(x, getBSTNODEvalue(y));
    setBSTNODEvalue(y, tmp);
}


/*
 *  Method (private):   isRoot
 *  Usage:  int isNodeRoot = t->isRoot(t, node);
 *  Description:
 */
int isRoot(BST *t, BSTNODE *n) {
    assert(t != 0 && n != 0);
    if (t->compare(getBSTNODEvalue(n), getBSTNODEvalue(t->root)) == 0) {
        return 1;
    }
    return 0;
}


/*
 *  Method (private):   isLeftChild
 *  Usage:  int isLeft = isLeftChild(n);
 *  Description: This private method returns true if the node is the left child
 *  of it's parent node.
 */
int isLeftChild(BST *t, BSTNODE *n) {
    assert(t != 0 && n != 0);
    if (t->isRoot(t, n)) return 0;
    BSTNODE *leftChild = getBSTNODEleft(getBSTNODEparent(n));
    if (leftChild == NULL) return 0;
    else if (t->compare(getBSTNODEvalue(n), getBSTNODEvalue(leftChild)) == 0) {
        return 1;
    }
    return 0;
}


/*
 *  Method (private):   isRightChild
 *  Usage:  int isRight = isRightChild(n);
 *  Description: This private method returns true if the node is the right child
 *  of it's parent node.
 */
int isRightChild(BST *t, BSTNODE *n) {
    assert(t != 0 && n != 0);
    if (t->isRoot(t, n)) return 0;
    BSTNODE *rightChild = getBSTNODEright(getBSTNODEparent(n));
    if (rightChild == NULL) return 0;
    if (t->compare(getBSTNODEvalue(n), getBSTNODEvalue(rightChild)) == 0) {
        return 1;
    }
    return 0;
}


/*
 *  Method (private):   getMinDepth
 *  Usage:  int minDepth = t->getMinDepth(t, t->root);
 *  Description:
 */
int getMinDepth(BST *t, BSTNODE *n) {
    assert(t != 0);
    if (n == NULL) return 0;
    if (getBSTNODEleft(n) == NULL || getBSTNODEright(n) == NULL) return 0;
    int leftDepth = t->getMinDepth(t, n->left) + 1;
    int rightDepth = t->getMinDepth(t, n->right) + 1;
    return (leftDepth < rightDepth) ? leftDepth : rightDepth;
}


/*
 *  Method (private):   getMaxDepth
 *  Usage:  int maxDepth = t->getMaxDepth(t, t->root);
 *  Description:
 */
int getMaxDepth(BST *t, BSTNODE *n) {
    assert(t != 0);
    if (t->root == NULL) return -1;
    if (n == NULL) return -1;
    int leftDepth = t->getMaxDepth(t, n->left);
    int rightDepth = t->getMaxDepth(t, n->right);
    return (leftDepth > rightDepth) ? leftDepth + 1 : rightDepth + 1;
}


/*
 *  Method (private):   getTreeMinimum
 *  Usage:  BSTNODE *minValueNode = getTreeMinimum(n);
 *  Description:
 */
BSTNODE *getTreeMinimum(BSTNODE *n) {
    while (getBSTNODEleft(n) != NULL) {
        n = getBSTNODEleft(n);
    }
    return n;
}


/*
 *  Method (private):   getTreeMaximum
 *  Usage:  BSTNODE *maxValueNode = getTreeMaximum(n);
 *  Description:
 */
BSTNODE *getTreeMaximum(BSTNODE *n) {
    while (getBSTNODEright(n) != NULL) {
        n = getBSTNODEright(n);
    }
    return n;
}


/*
 *  Method (private):   getSuccessor
 *  Usage:  BSTNODE *successor = t->getSuccessor(t, n);
 *  Description:
 */
BSTNODE *getSuccessor(BST *t, BSTNODE *n) {
    // TODO: Am I correct!
    assert(t != 0);
    assert(n != 0);
    if (getBSTNODEright(n) != NULL) {
        return getTreeMinimum(getBSTNODEright(n));
    }
    if (t->compare != NULL) {
        BSTNODE *y = getBSTNODEparent(n);
        while (y != NULL && t->compare(n->value, getBSTNODEvalue(y->right)) == 0) {
            n = y;
            y = getBSTNODEparent(y);
        }
        return y;
    }
    else {
        return getTreeMaximum(getBSTNODEleft(n));
    }
}


/*
 *  Method (private):   getPredecessor
 *  Usage:  BSTNODE *pred = t->getPredecessor(t, n);
 *  Description:
 */
BSTNODE *getPredecessor(BST *t, BSTNODE *n) {
    // TODO: Am I correct!
    assert(t != 0);
    assert(n != 0);
    if (getBSTNODEleft(n) != NULL) {
        return getTreeMaximum(getBSTNODEleft(n));
    }
    if (t->compare != NULL) {
        BSTNODE *y = getBSTNODEparent(n);
        while ( y != NULL && t->compare(n->value, getBSTNODEvalue(y->left)) == 0) {
            n = y;
            y = getBSTNODEparent(y);
        }
        return y;
    }
    else {
        return getTreeMinimum(getBSTNODEright(n));
    }
}


/*
 *  Method (private):
 *  Usage:
 *  Description:
 */
void displayPreorder(BST *t, BSTNODE *n, FILE *fp) {
    // TODO: Am I correct?
    // TODO: Am I efficient?
    assert(t != 0);
    if (n == NULL) return;
    t->display(getBSTNODEvalue(n), fp);
    if (n->left != NULL) fprintf(fp, " [");
    t->displayPreorder(t, n->left, fp);
    if (n->left != NULL) fprintf(fp, "]");
    if (n->right != NULL) fprintf(fp, " [");
    t->displayPreorder(t, n->right, fp);
    if (n->right != NULL) fprintf(fp, "]");
}


/*
 *  Method (private):
 *  Usage:
 *  Description:
 */
void freeTree(BST *t, BSTNODE *n) {
    if (n == NULL) return;
    t->freeTree(t, n->left);
    t->freeTree(t, n->right);
    freeBSTNODE(n, t->free);
}
