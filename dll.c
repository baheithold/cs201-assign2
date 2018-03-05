/*
 * File:    dll.c
 * Author:  Brett Heithold
 * Description: This is the implementation file for the DLL (doubly-linked list)
 * class.
 */


#include "dll.h"
#include <stdlib.h>
#include <assert.h>


/*
 *  Type: NODE
 *  Description: This is the NODE struct used to construct the doubly-linked
 *  list data structure.
 */
typedef struct node {
    void *value;
    struct node *next;
    struct node *prev;
} NODE;

/*
 *  Constructor: newNODE
 *  Usage:  NODE *n = newNODE(value, n, p);
 *  Description: This constructor instantiates a new NODE object.
 */
NODE *newNODE(void *value, NODE *next, NODE *prev) {
    NODE *n = malloc(sizeof(NODE));
    assert(n != 0);
    n->value = value;
    n->next = next;
    n->prev = prev;
    return n;
}

/*
 * Method:  getNODEvalue
 * Usage:   void *value = getNODEvalue(n);
 * Description: This method returns the generic value stored in a NODE struct.
 */
void *getNODEvalue(NODE *n) {
    return n->value;
}

/*
 *  Method: setNODEvalue
 *  Usage:  void *value = setNODEvalue(n, newValue);
 *  Description: This method sets the value of a NODE. The method returns the
 *  old value.
 */
void *setNODEvalue(NODE *n, void *value) {
    void *oldValue = getNODEvalue(n);
    n->value = value;
    return oldValue;
}

/*
 *  Method: getNODEnext
 *  Usage:  NODE *next = getNODEnext(n);
 *  Description: This method returns the next pointer of a NODE struct.
 */
NODE *getNODEnext(NODE *n) {
    return n->next;
}

/*
 *  Method: setNODEnext
 *  Usage:  setNODEnext(n, newNext);
 *  Description: This method sets the next pointer of a NODE struct with a new
 *  NODE struct.
 */
void setNODEnext(NODE *n, NODE *newNext) {
    assert(n != 0);
    n->next = newNext;
}

/*
 *  Method: getNODEprev
 *  Usage:  NODE *prev = getNODEprev(n);
 *  Description: This method returns the prev pointer of a NODE struct.
 */
NODE *getNODEprev(NODE *n) {
    return n->prev;
}

/*
 *  Method: setNODEprev
 *  Usage:  setNODEprev(n, newPrev);
 *  Description: This method sets the prev pointer of a NODE struct with a new
 *  NODE struct.
 */
void setNODEprev(NODE *n, NODE *newPrev) {
    assert(n != 0);
    n->prev = newPrev;
}


// Private DLL method prototypes
static void addToFront(DLL *items, void *value);
static void addToBack(DLL *items, void *value);
static void insertAtIndex(DLL *items,int, void *value);
static void *removeFromFront(DLL *items);
static void *removeFromBack(DLL *items);
static void *removeFromIndex(DLL *items, int index);
static NODE *getNodeAtIndex(DLL *items, int index);


/*
 *  Type:   DLL
 *  Description: This DLL struct is the basis of the DLL class. 
 *  This doubly-linked list is a linear doubly-linked list with both head and
 *  tail pointers.
 */
struct DLL {
    NODE *head;
    NODE *tail;
    int size;

    // Public methods
    void (*display)(void *, FILE *);
    void (*free)(void *);
    
    // Private methods
    void (*addToFront)(DLL *, void *);
    void (*addToBack)(DLL *, void *);
    void (*insertAtIndex)(DLL *,int, void *);
    void *(*removeFromFront)(DLL *);
    void *(*removeFromBack)(DLL *);
    void *(*removeFromIndex)(DLL *, int);
    NODE *(*getNodeAtIndex)(DLL *, int);
};

/*
 *  Constructor:    newDLL
 *  Usage:  DLL *items = newDLL(displayINTEGER, freeINTEGER);
 *  Description: This is the constructor for the DLL class. This constructor
 *  instantiates a new DLL object.
 */
DLL *newDLL(void (*d)(void *, FILE *), void (*f)(void *)) {
    DLL *items = malloc(sizeof(DLL));
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
    items->getNodeAtIndex = getNodeAtIndex;
    return items;
}

/*
 *  Method: insertDLL
 *  Usage:  insertDLL(items, index, value);
 *  Description: This method inserts a generic value into a DLL object. This 
 *  method can calls private helper methods to insert the value at the head,
 *  tail, or at a given index of a DLL object. It runs in constant time for
 *  insertions at a constant distance from the front and from the back.
 *  The doubly-linked list uses zero-based indexing. 
 */
void insertDLL(DLL *items, int index, void *value) {
    assert(items != 0);
    assert(index >= 0 && index <= items->size);
    if (index == 0) {
        // Value is to be added at the front of the list
        items->addToFront(items, value);
    }
    else if (index == items->size) {
        // Value is to be added at the back of the list
        items->addToBack(items, value);
    }
    else {
        // Value is to be inserted at an index between 1 and items->size - 1
        items->insertAtIndex(items, index, value);
    }
}

/*
 *  Method: removeDLL
 *  Usage:  void *value = removeDLL(items, index);
 *  Description: This method removes values at a given index. It runs in
 *  constant time for removals at a constant distance from the front
 *  and from the back.
 */
void *removeDLL(DLL *items, int index) {
    assert(items != 0);
    assert(items->size > 0);
    assert(index >= 0 && index < items->size);
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
 *  Method: unionDLL
 *  Usage:  unionDLL(recipient, donor);
 *  Description: The union method takes two lists and moves all the items 
 *  in the donor list to the recipient list. If the recipient list has the 
 *  items {{3,4,5}} and the donor list has the items {{1,2}}, then, after the 
 *  union, the donor list will be empty and recipient list will have the 
 *  items {{3,4,5,1,2}}. The union method runs in constant time.  
 */
void unionDLL(DLL *recipient, DLL *donor) {
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
        setNODEnext(recipient->tail, donor->head);
        setNODEprev(donor->head, recipient->tail);
        recipient->tail = donor->tail;
        recipient->size += donor->size;
    }
        donor->head = NULL;
        donor->tail = NULL;
        donor->size = 0;
}


/* Method:  getDLL
 * Usage:   void *value = getDLL(items, index);
 * Description: The method returns the value at the given index. 
 * It runs in constant time for retrievals at a constant distance from the 
 * front and from the back.
 */
void *getDLL(DLL *items, int index) {
    // TODO: Can I do better?
    assert(items != 0);
    assert(index >= 0 && index < items->size);
    if (index == 0) {
        return getNODEvalue(items->head);
    }
    else if (index == items->size - 1) {
        return getNODEvalue(items->tail);
    }
    else {
        NODE *curr = items->getNodeAtIndex(items, index);
        return getNODEvalue(curr);
    }
}

/*
 *  Method: setDLL
 *  Usage:  void *value = setDLL(items, index, value);
 *  Description: The method updates the value at the given index. If the given
 *  index is a valid index for the list, the replaced value is returned.
 *  If the given index is equal to the size, the value is appended to the list
 *  and a null pointer is returned. It runs in constant time for updates at a
 *  constant distance from the front and from the back.
 */
void *setDLL(DLL *items, int index, void *value) {
    // TODO: Am I right? Can I do better?
    assert(items != 0);
    assert(index >= 0 && index <= items->size);
    void *oldValue = NULL;
    if (index == 0) {
        // set value of head
        oldValue = setNODEvalue(items->head, value);
    }
    else if (index == items->size - 1) {
        // set value of tail
        oldValue = setNODEvalue(items->tail, value);
    }
    else if (index == items->size) {
        // Add new NODE to end of list
        items->addToBack(items, value);
    }
    else {
        NODE *n = items->getNodeAtIndex(items, index);
        oldValue = setNODEvalue(n, value);
    }
    return oldValue;
}

/*
 *  Method: sizeDLL
 *  Usage:  int size = sizeDLL(items);
 *  Description: This method returns the number of values stored in 
 *  a DLL object. This method runs in constant time.
 */
int sizeDLL(DLL *items) {
    assert(items != 0);
    return items->size;
}

/*
 *  Method: displayDLL
 *  Usage:  displayDLL(items, stdout);
 *  Example Output: {{5,6,2,9,1}}
 */
void displayDLL(DLL *items, FILE *fp) {
    assert(items != 0);
    fprintf(fp, "{{");
    NODE *curr = items->head;
    while (curr != NULL) {
        items->display(getNODEvalue(curr), fp);
        curr = getNODEnext(curr);
        if (curr != NULL) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "}}");
}

/*
 *  Method: displayDLLdebug
 *  Usage:  displayDLLdebug(items, stdout);
 *  Example Output: head->{{5,6,2,9,1}},tail->{{1}}
 */
void displayDLLdebug(DLL *items, FILE *fp) {
    assert(items != 0);
    fprintf(fp, "head->{{");
    NODE *curr = items->head;
    while (curr != NULL) {
        items->display(getNODEvalue(curr), fp);
        if (getNODEnext(curr) != NULL) {
            fprintf(fp, ",");
        }
        curr = getNODEnext(curr);
    }
    fprintf(fp, "}},tail->{{");
    if (items->size > 0) {
        items->display(getNODEvalue(items->tail), fp);
    }
    fprintf(fp, "}}");
}

/*
 *  Method: freeDLL
 *  Usage:  freeDLL(items);
 *  Description: This method walks through the list, freeing the generic
 *  values (using the passed-in freeing function) and the nodes that hold them.
 *  If the freeing function is null, the generic value is not freed.
 */
void freeDLL(DLL *items) {
    assert(items != 0);
    NODE *curr = items->head;
    NODE *tmp;
    while (curr != NULL) {
        if (items->free != NULL) {
            items->free(getNODEvalue(curr));
        }
        tmp = curr;
        curr = getNODEnext(curr);
        free(tmp);
    }
    free(items);
}


/************************* Private Methods **************************/

/*
 *  Method (private):   addToFront
 *  Usage:  addToFront(items, value);
 *  Description: This method adds a value to the head of a DLL object.
 */
void addToFront(DLL *items, void *value) {
    assert(items != 0);
    NODE *n = newNODE(value, items->head, NULL);
    if (items->size == 0) {
        // List is empty, set head and tail to same node
        items->head = n;
        items->tail = n;
    }
    else {
        // List is non-empty
        setNODEprev(items->head, n);
        items->head = n;
    }
    items->size++;
}

/*
 *  Method (private):   addToBack
 *  Usage:  addToBack(items, value);
 *  Description: This method adds a value to the tail of a DLL object.
 */
void addToBack(DLL *items, void *value) {
    assert(items != 0);
    if (items->size == 0) {
        // List is empty
        items->addToFront(items, value);
    }
    else {
        NODE *n = newNODE(value, NULL, items->tail);
        setNODEnext(items->tail, n);
        items->tail = n;
    }
    items->size++;
}

/*
 *  Method (private):   insertAtIndex
 *  Usage:  insertAtIndex(items, index, value);
 *  Description: This method inserts a value at a given index of a DLL object.
 */
void insertAtIndex(DLL *items, int index, void *value) {
    assert(items != 0);
    assert(index >= 0 && index <= items->size);
    if (index == 0) {
        items->addToFront(items, value);
    }
    else if (index == items->size) {
        items->addToBack(items, value);
    }
    else {
        // get node prev to node at index
        NODE *curr = items->getNodeAtIndex(items, index - 1);
        NODE *n = newNODE(value, curr->next, curr);
        setNODEprev(curr->next, n);
        setNODEnext(curr, n);
        items->size++;
    }
}

/*
 *  Method (private):   removeFromFront
 *  Usage:  void *value = removeFromFront(items);
 *  Description: This method removes and returns the generic value stored 
 *  at the head of a DLL object.
 */
void *removeFromFront(DLL *items) {
    assert(items != 0);
    NODE *oldHead = items->head;
    void *oldValue = getNODEvalue(oldHead);
    items->head = getNODEnext(oldHead);
    items->size--;
    if (items->size == 0) {
        // List is going empty
        items->tail = NULL;
    }
    free(oldHead);
    return oldValue;
}

/*
 *  Method (private):   removeFromBack
 *  Usage:  void *value = removeFromBack(items);
 *  Description: This method removes and returns the generic value stored 
 *  at the tail of a DLL object.
 */
void *removeFromBack(DLL *items) {
    assert(items != 0);
    void *oldValue;
    if (items->size == 1) {
        // List is going empty
        oldValue = items->removeFromFront(items);
    }
    else {
        NODE *curr = getNODEprev(items->tail); // get next to last node
        oldValue = getNODEvalue(items->tail);
        free(items->tail);
        items->tail = curr;
        setNODEnext(curr, NULL);
        items->size--;
    }
    return oldValue;
}

/*
 *  Method (private):   removeFromIndex
 *  Usage:  void *value = removeFromIndex(items, index);
 *  Description: This is a helper method for the removeDLL method that returns
 *  the value at the given index and removes the NODE from that index.
 */
void *removeFromIndex(DLL *items, int index) {
    assert(items != 0);
    void *oldValue;
    if (index == 0) {
        oldValue = items->removeFromFront(items);
    }
    else if (index == items->size - 1) {
        oldValue = items->removeFromBack(items);
    }
    else {
        // get node prev to index
        NODE *curr = items->getNodeAtIndex(items, index - 1);
        NODE *oldNode = getNODEnext(curr);
        oldValue = getNODEvalue(oldNode);
        setNODEnext(curr, getNODEnext(oldNode));
        setNODEprev(getNODEnext(oldNode), curr);
        items->size--;
        if (items->size == 0) {
            // List went empty
            items->head = NULL;
            items->tail = NULL;
        }
        free(oldNode);
    }
    return oldValue;
}

/*
 *  Method (private): getNodeAtIndex
 *  Usage   NODE *n = getNodeAtIndex(items, index);
 *  Description: This method returns the NODE at the given index.
 */
NODE *getNodeAtIndex(DLL *items, int index) {
    // FIXME
    NODE *curr;
    if (items->size - index <= index) {
        // traverse from the tail of the list
        curr = items->tail;
        int newIndex = items->size - index - 1;
        while (newIndex > 0) {
            curr = getNODEprev(curr);
            newIndex--;
        }
    }
    else {
        // traverse from the head of the list
        curr = items->head;
        while (index > 0) {
            curr = getNODEnext(curr);
            index--;
        }
    }
    return curr;
}
