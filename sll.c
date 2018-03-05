/*
 *  File:   sll.c
 *  Author: Brett Heithold
 *  Description: This is the implementation file for the
 *  sll (singly-linked list) class.
 */


#include "sll.h"
#include <stdlib.h>
#include <assert.h>


/*
 *  Type: NODE
 *  Description: This is the NODE struct used for the implementation
 *  of the SLL class.
 */
typedef struct node {
    void *value;
    struct node *next;
} NODE;


/*
 *  Constructor: newNODE
 *  Usage: NODE *n = newNODE(value, next);
 *  Description: This constructor creates a new NODE object with a value
 *  and initializes the node's next pointer.
 */
NODE *newNODE(void *value, NODE *next) {
    NODE *n = malloc(sizeof(NODE));
    assert(n != 0);
    n->value = value;
    n->next = next;
    return n;
}


// Private SLL method prototypes
static void addToFront(SLL *items, void *value);
static void addToBack(SLL *items, void *value);
static void insertAtIndex(SLL *items, int index, void *value);
static void *removeFromFront(SLL *items);
static void *removeFromBack(SLL *items);
static void *removeFromIndex(SLL *items, int index);


/*
 *  Type: SLL
 *  Description: This is the SLL struct used for basis of the SLL class.
 */
struct SLL {
    NODE *head;
    NODE *tail;
    int size;

    // Public Methods
    void (*display)(void *, FILE *);
    void (*free)(void *);

    // Private Methods
    void (*addToFront)(SLL *, void *);
    void (*addToBack)(SLL *, void *);
    void (*insertAtIndex)(SLL *, int, void *);
    void *(*removeFromFront)(SLL *);
    void *(*removeFromBack)(SLL *);
    void *(*removeFromIndex)(SLL *, int);
}; 


/*
 *  Constructor: newSLL
 *  Usage: SLL *s = newSLL(displayINTEGER, freeINTEGER);
 *  Description: This constructor initializes a new SLL object. It sets the
 *  head and tail pointers to NULL as well as the size to 0. This constructor
 *  also sets the function pointers.
 */
SLL *newSLL(void (*d)(void *, FILE *), void (*f)(void *)) {
    SLL *items = malloc(sizeof(SLL));
    assert(items != 0);
    items->head = NULL;
    items->tail = NULL;
    items->size = 0;
    items->display = d;
    items->free = f;
    items->addToFront = addToFront;
    items->addToBack = addToBack;
    items->insertAtIndex = insertAtIndex;
    items->removeFromFront = removeFromFront;
    items->removeFromBack = removeFromBack;
    items->removeFromIndex = removeFromIndex;
    return items;
}


/*
 *  Method: insertSLL
 *  Usage: insertSLL(s, i, v);
 *  Description: This method inserts a value into the list at a given index.
 *  It runs in constant time for insertions at the very end of the list
 *  and runs in constant time for insertions that are a constant distance
 *  from the front. The singly-linked list uses zero-based indexing.
 */
void insertSLL(SLL *items, int index, void *value) {
    assert(items != 0);
    assert(index >= 0 && index <= items->size);
    if (index == 0) {
        // Node is to be added to the front of the list
        items->addToFront(items, value);
    }
    else if (index == items->size) {
        // Node is to be added to the back of the list
        items->addToBack(items, value);
    }
    else {
        // Node is to be inserted at an index between 0 and items->size
        items->insertAtIndex(items, index, value);
    }
}


/*
 *  Method: removeSLL
 *  Usage: void *val = removeSLL(list, index);
 *  Description:
 */
void *removeSLL(SLL *items, int index) {
    assert(items != 0);
    assert(items->size > 0 && index >= 0 && index < items->size);
    void *oldValue;
    if (index == 0) {
        // Remove from front
        oldValue = items->removeFromFront(items);
    }
    else if (index == items->size - 1) {
        // Remove from back
        oldValue = items->removeFromBack(items);
    }
    else {
        // Remove from index
        oldValue = items->removeFromIndex(items, index);
    }
    return oldValue;
}


/*
 *  Method: unionSLL
 *  Usage: unionSLL(recipient, donor);
 *  Description: The union method takes two lists and moves all the items in the
 *  donor list to the recipient list. If the recipient list has the items
 *  {3,4,5} and the donor list has the items {1,2}, then, after the union,
 *  the donor list will be empty and recipient list will have the items
 *  {3,4,5,1,2}. The union method runs in constant time. 
 */
void unionSLL(SLL *recipient, SLL *donor) {
    // TODO: Do I work correctly? I THINK
    assert(recipient != 0 && donor != 0);
    if (recipient->size == 0 && donor->size == 0) {
        return;
    }
    else if (recipient->size > 0 && donor->size == 0) {
        return;
    }
    else if (recipient->size == 0 && donor->size > 0) {
        recipient->head = donor->head;
        recipient->tail = donor->tail;
        recipient->size = donor->size;
    }
    else {
        recipient->tail->next = donor->head;
        recipient->tail = donor->tail;
        recipient->size += donor->size;
    }
    donor->head = NULL;
    donor->tail = NULL;
    donor->size = 0;
}


/*
 *  Method: getSLL
 *  Usage:  void *value = getSLL(list, index);
 *  Description: This method returns the value at the given index. It runs in
 *  constant time for retrievals at the very back of the list and at a constant
 *  distance from the front of the list. The given index must be greater than
 *  or equal to zero and less than the size of the list.
 */
void *getSLL(SLL *items, int index) {
    // TODO: Do I Work Right?
    assert(index >= 0 && index < items->size);
    if (index == 0) {
        return items->head->value;
    }
    else if (index == items->size - 1) {
        return items->tail->value;
    }
    else {
        NODE *curr = items->head;
        while (index > 0) {
            curr = curr->next;
            index--;
        }
        return curr->value;
    }
}


/*
 *  Method: setSLL
 *  Usage: void *oldValue = setSLL(list, index, value);
 *  Description: This method updates the value at the given index. If the given
 *  index is a valid index for the list, the replaced value is returned. If the
 *  given index is equal to the size, the value is appended to the list and a 
 *  NULL pointer is returned. This method runs in constant time for updates at 
 *  the very back of the list and at a constant distance from the front.
 *  A valid index for the list must be greater than or equal to zero and
 *  less than or equal to the size of the list.
 */
void *setSLL(SLL *items, int index, void *value) {
    // TODO: Do I Work Right?
    assert(index >= 0 && index <= items->size);
    void *oldValue = NULL;
    if (index == items->size - 1) {
        // Replace value at tail
        oldValue = items->tail->value;
        items->tail->value = value;
    }
    else if (index == items->size) {
        // Append value to the end of the list
        items->addToBack(items, value);
    }
    else {
        NODE *curr = items->head;
        while (index > 0) {
            curr = curr->next;
            index--;
        }
        oldValue = curr->value;
        curr->value = value;
    }
    return oldValue;
}


/*
 *  Method: sizeSLL
 *  Usage: int size = sizeSLL(list);
 *  Description: This method returns the number of items stored in the list.
 */
int sizeSLL(SLL *items) {
    assert(items != 0);
    return items->size;
}


/*
 *  Method: displaySLL
 *  Usage: displaySLL(list, stdout);
 *  Description: This method prints the items stored in the list to the 
 *  file pointer passed as an argument. There is no preceeding or following
 *  whitespace printed.
 *  Example: If the list holds the integers 5, 6, 2, 9, and 1 from front to back
 *           {5,6,2,9,1}
 *           Empty-List
 *           {}
 */
void displaySLL(SLL *items, FILE *fp) {
    assert(items != 0);
    fprintf(fp, "{");
    NODE *curr = items->head;
    while (curr != NULL) {
        items->display(curr->value, fp);
        if (curr->next != NULL) {
            fprintf(fp, ",");
        }
        curr = curr->next;
    }
    fprintf(fp, "}");
}


/*
 *  Method: displaySLLdebug
 *  Usage: displaySLLdebug(list, stdout);
 *  Example Output: head->{5,6,2,9,1},tail->{1}
 */
void displaySLLdebug(SLL *items, FILE *fp) {
    assert(items != 0);
    // Display head
    fprintf(fp, "head->{");
    NODE *curr = items->head;
    while (curr != NULL) {
        items->display(curr->value, fp);
        if (curr->next != NULL) {
            fprintf(fp, ",");
        }
        curr = curr->next;
    }
    fprintf(fp, "},");
    // Display tail
    fprintf(fp, "tail->{");
    if (items->size != 0) {
        items->display(items->tail->value, fp);
    }
    fprintf(fp, "}");
}


/*
 *  Method: freeSLL
 *  Usage: freeSLL(list);
 *  Description: This method walks through the list, freeing the generic
 *  values (using the passed-in freeing function) and the nodes that hold them.
 *  If the freeing function is null, the generic value is not freed.
 */
void freeSLL(SLL *items) {
    // TODO: Do I Work Right?
    assert(items != 0);
    NODE *curr = items->head;
    NODE *tmp;
    while (curr != NULL) {
        if (items->free != NULL) {
            items->free(curr->value);
        }
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    free(items);
}


/************************* Private Methods **************************/

void addToFront(SLL *items, void *value) {
    assert(items != 0);
    items->head = newNODE(value, items->head);
    if (items->size == 0) {
        // List was empty before insertion
        items->tail = items->head;
    }
    items->size++;
}


void addToBack(SLL *items, void *value) {
    assert(items != 0);
    if (items->size == 0) {
        // List is empty
        items->addToFront(items, value);
    }
    else {
        items->tail->next = newNODE(value, NULL);
        items->tail = items->tail->next;
        items->size++;
    }
}


void insertAtIndex(SLL *items, int index, void *value) {
    assert(items != 0);
    if (index == 0) {
        items->addToFront(items, value);
    }
    else if (index == items->size) {
        items->addToBack(items, value);
    }
    else {
        NODE *curr = items->head;
        while (index > 1) {
            curr = curr->next;
            index--;
        }
        NODE *n = newNODE(value, curr->next);
        curr->next = n;
        items->size++;
    }
}


void *removeFromFront(SLL *items) {
    assert(items != 0);
    void *oldValue = items->head->value;
    NODE *tmp = items->head;
    items->head = items->head->next;
    items->size--;
    if (items->size == 0) {
        items->tail = NULL;
    }
    free(tmp);
    return oldValue;
}


void *removeFromBack(SLL *items) {
    void *oldValue;
    assert(items != 0);
    if (items->size == 1) {
        oldValue = items->removeFromFront(items);
    }
    else {
        NODE *curr = items->head;
        while (curr->next->next != NULL) {
            curr = curr->next;
        }
        NODE *tmp = curr->next;
        oldValue = tmp->value;
        curr->next = NULL;
        free(tmp);
        items->tail = curr;
        items->size--;
    }
    return oldValue;
}


void *removeFromIndex(SLL *items, int index) {
    // TODO: Do I Work Right?
    void *oldValue;
    NODE *curr = items->head;
    while (index > 1) {
        curr = curr->next;
        index--;
    }
    NODE *oldNode = curr->next;
    oldValue = oldNode->value;
    curr->next = oldNode->next;
    items->size--;
    if (items->size == 0) {
        items->head = NULL;
        items->tail = NULL;
    }
    free(oldNode);
    return oldValue;
}
