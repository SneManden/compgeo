#include "rbltree.h"
#include <stdio.h>
#include <stdlib.h> // malloc
#include <assert.h>

// Forward references
void RBLleftRotate(RBTree *T, RBNode *x);
void RBLrightRotate(RBTree *T, RBNode *x);
void RBLinsertFixup(RBTree *T, RBNode *z);
void RBLtransplant(RBTree *T, RBNode *u, RBNode *v);
void RBLdeleteFixup(RBTree *T, RBNode *x);

static int nilnum = 0;


// Search tree operations
RBLNode *RBLtreeSearch(RBLTree *T, RBLNode *x, int k) {
    return RBLtreeSearch(T, x, k);
}
RBLNode *RBLtreeSearchIterative(RBLTree *T, RBLNode *x, int k) {
    return RBtreeSearchIterative(T, x, k);
}
RBLNode *RBLtreeMinimum(RBLTree *T, RBLNode *x) {
    return RBtreeMinimum(T, x);
}
RBLNode *RBLtreeMaximum(RBLTree *T, RBLNode *x) {
    return RBtreeMaximum(T, x);
}
RBLNode *RBLtreeSuccessor(RBLTree *T, RBLNode *x) {
    return RBtreeSuccessor(T, x);
}
RBLNode *RBLtreePredecessor(RBLTree *T, RBLNode *x) {
    return RBtreePredecessor(T, x);
}


// Common methods
RBLTree *RBLinit() {
    return RBinit();
}

RBLNode *RBLnewNode(int key, void *data) {
    return RBnewNode(key, data);
}

void RBLinsert(RBLTree *T, RBLNode *z) {
    RBLNode *y = T->nil;
    RBLNode *x = T->root;
    while (x != T->nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->p = y;
    if (y == T->nil)
        T->root = z;
    else {
        // Create sibling with parent's data
        RBLNode *p = RBLnewNode(y->key, y->data);
        p->p = y;
        if (z->key < y->key) {      // z is a left child
            y->left = z;
            // insert p as right child
            assert(y->right == T->nil);
            y->right = p;
            y->key = z->key;
        } else {                    // z is a right child
            y->right = z;
            // insert p as left child
            assert(y->left == T->nil);
            y->left = p;
        }
        y->data = NULL;
        p->left = T->nil;
        p->right = T->nil;
        p->color = RED;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    RBLinsertFixup(T, z);
}

void RBLdelete(RBLTree *T, RBLNode *z) {
    RBdelete(T, z);
}


// Helper methods
void RBLleftRotate(RBTree *T, RBNode *x) {
    RBleftRotate(T, x);
}
void RBLrightRotate(RBTree *T, RBNode *x) {
    RBrightRotate(T, x);
}
void RBLinsertFixup(RBTree *T, RBNode *z) {
    RBinsertFixup(T, z);
}
void RBLtransplant(RBTree *T, RBNode *u, RBNode *v) {
    RBtransplant(T, u, v);
}
void RBLdeleteFixup(RBTree *T, RBNode *x) {
    RBdeleteFixup(T, x);
}


// testing methods
int RBLeachLeafIsBlack(RBLTree *T) {
    return RBeachLeafIsBlack(T);
}
int RBLeachRedNodeHasBlackChildren(RBLTree *T) {
    return RBeachRedNodeHasBlackChildren(T);
}
int RBLeachRootLeafPathHasEqualLength(RBLTree *T) {
    return RBeachRootLeafPathHasEqualLength(T);
}
int RBLisRBLTree(RBLTree *T) {
    return RBisRBTree(T);
}


// miscelanous
void RBLwriteNil(FILE *fd, RBNode *x) {
    int id = nilnum++;
    fprintf(fd, "  nil%d [label=nil color=black shape=box width=0.25 height=0.25 fontsize=10]\n", id);
    fprintf(fd, "  n%p -> nil%d\n", (void*)x, id);
}
void RBLwriteTreeNode(FILE *fd, RBTree *T, RBNode *x) {
    if (x != T->nil) {
        // Print node
        fprintf(fd, "  n%p [label=%d color=%s fontcolor=%s];\n",
            (void*)x, x->key, (x->color==RED ? "firebrick":"black"),
            (x->data==NULL ? "black":"silver"));
        // left
        if (RBLhasLeft(T, x))
            fprintf(fd, "  n%p -> n%p\n", (void*)x, (void*)x->left);
        else
            RBLwriteNil(fd, x);
        // right
        if (RBLhasRight(T, x))
            fprintf(fd, "  n%p -> n%p\n", (void*)x, (void*)x->right);
        else
            RBLwriteNil(fd, x);
        // Recursive
        RBLwriteTreeNode(fd, T, x->left);
        RBLwriteTreeNode(fd, T, x->right);
    }
}
void RBLwriteTree(RBLTree *T, char *filename) {
    FILE *fd = fopen(filename, "w");
    fprintf(fd, "digraph {\n");
    fprintf(fd, "  nodesep=0.3;\n");
    fprintf(fd, "  ranksep=0.2;\n");
    fprintf(fd, "  node [shape=circle style=filled fontcolor=white];\n");
    fprintf(fd, "  edge [arrowsize=0.8];\n");
    // fprintf(fd, " n%p [label=nil shape=box color=black];", x);
    RBwriteTreeNode(fd, T, T->root);
    fprintf(fd, "}\n");
    fclose(fd);
}
