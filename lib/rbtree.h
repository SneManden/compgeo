#ifndef __RBTREE_H
#define __RBTREE_H

typedef enum {RED, BLACK} RBColor;

typedef struct RBNode {
    int key;
    void *data;
    struct RBNode *p; // parent
    struct RBNode *left;
    struct RBNode *right;
    RBColor color;
} RBNode;

typedef struct RBTree {
    struct RBNode *root;
    RBNode *nil;
} RBTree;

// macros
#define RBisLeaf(T, x) ( (x) == (T)->nil )
#define RBhasLeft(T, x) ( (x)->left != (T)->nil )
#define RBhasRight(T, x) ( (x)->right != (T)->nil )
#define RBhasColor(x, col) ( (x)->color == col )
#define RBisEmpty(T) ( (T)->root == (T)->nil )
// Search tree operations
RBNode *RBtreeSearch(RBTree *T, RBNode *x, int k);
RBNode *RBtreeSearchIterative(RBTree *T, RBNode *x, int k);
RBNode *RBtreeMinimum(RBTree *T, RBNode *x);
RBNode *RBtreeMaximum(RBTree *T, RBNode *x);
RBNode *RBtreeSuccessor(RBTree *T, RBNode *x);
RBNode *RBtreePredecessor(RBTree *T, RBNode *x);
// common methods
RBTree *RBinit();
RBNode *RBnewNode(int key, void *data);
void RBinsert(RBTree *T, RBNode *z);
void RBdelete(RBTree *T, RBNode *z);
// testing methods
int RBeachLeafIsBlack(RBTree *T);
int RBeachRedNodeHasBlackChildren(RBTree *T);
int RBeachRootLeafPathHasEqualLength(RBTree *T);
int RBisRBTree(RBTree *T);
// miscelanous
void RBwriteTree(RBTree *T, char *filename);

#endif /* __RBTREE_H */