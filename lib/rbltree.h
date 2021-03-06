#ifndef __RBLTREE_H
#define __RBLTREE_H

/**
 * Leaf oriented RedBlack search tree:
 *   - Data are stored in the leaves of the tree.
 *   - The leaves are also a doubly linked list with
 *     pointers to successor/predecessor leaves.
 */

#include "util.h"

typedef enum {RED, BLACK} RBLColor;

typedef struct RBLNode {
    int key;
    void *data;
    struct RBLNode *p; // parent
    struct RBLNode *left;
    struct RBLNode *right;
    struct RBLNode *prev;
    struct RBLNode *next;
    RBLColor color;
} RBLNode;
typedef struct RBLTree {
    struct RBLNode *root;
    RBLNode *nil;
} RBLTree;

// macros
#define RBLisLeaf(T, x) ( (x) == (T)->nil )
#define RBLhasLeft(T, x) ( (x)->left != (T)->nil )
#define RBLhasRight(T, x) ( (x)->right != (T)->nil )
#define RBLhasColor(x, col) ( (x)->color == col )
#define RBLisEmpty(T) ( (T)->root == (T)->nil )
#define RBLhasData(x) ( (x)->data != NULL )
// Search tree operations
RBLNode *RBLtreeSearch(RBLTree *T, int k);
RBLNode *RBLtreeSearchIterative(RBLTree *T, int k);
RBLNode *RBLtreeMinimum(RBLTree *T, RBLNode *x);
RBLNode *RBLtreeMaximum(RBLTree *T, RBLNode *x);
RBLNode *RBLtreeSuccessor(RBLTree *T, RBLNode *x);
RBLNode *RBLtreePredecessor(RBLTree *T, RBLNode *x);
// common methods
RBLTree *RBLinit();
RBLNode *RBLnewNode(int key, void *data);
void RBLinsert(RBLTree *T, RBLNode *z);
void RBLdelete(RBLTree *T, RBLNode *z);
// testing methods
int RBLeachLeafIsBlack(RBLTree *T);
int RBLeachRedNodeHasBlackChildren(RBLTree *T);
int RBLeachRootLeafPathHasEqualLength(RBLTree *T);
int RBLisRBLTree(RBLTree *T);
int RBLisRBLTreeVerbose(RBLTree *T);
// miscelanous
void RBLwriteTree(RBLTree *T, char *filename);
void RBLdestroy(RBLTree *T, RBLNode *x);
void RBLtreeDestroy(RBLTree *T);

#endif /* __RBLTREE_H */