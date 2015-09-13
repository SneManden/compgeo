#include "rbltree.h"
#include <stdio.h>
#include <stdlib.h> // malloc
#include <assert.h>

// Forward references
void RBLleftRotate(RBLTree *T, RBLNode *x);
void RBLrightRotate(RBLTree *T, RBLNode *x);
void RBLinsertFixup(RBLTree *T, RBLNode *z);
void RBLtransplant(RBLTree *T, RBLNode *u, RBLNode *v);
void RBLdeleteFixup(RBLTree *T, RBLNode *x);

static int nilnum = 0;


// Search tree operations
RBLNode *RBLtreeSearchWorker(RBLTree *T, RBLNode *x, int k) {
    if (x == T->nil || k == x->key)
        return x;
    if (k < x->key)
        return RBLtreeSearchWorker(T, x->left, k);
    else
        return RBLtreeSearchWorker(T, x->right, k); 
}
RBLNode *RBLtreeSearch(RBLTree *T, int k) {
    return RBLtreeSearchWorker(T, T->root, k);
}
RBLNode *RBLtreeSearchIterative(RBLTree *T, int k) {
    RBLNode *x = T->root;
    while (x != T->nil && k != x->key) {
        if (k < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}
RBLNode *RBLtreeMinimum(RBLTree *T, RBLNode *x) {
    while (x != T->nil && x->left != T->nil)
        x = x->left;
    return x;
}
RBLNode *RBLtreeMaximum(RBLTree *T, RBLNode *x) {
    while (x != T->nil && x->right != T->nil)
        x = x->right;
    return x;
}
RBLNode *RBLtreeSuccessor(RBLTree *T, RBLNode *x) {
    if (x->right != T->nil)
        return RBLtreeMinimum(T, x->right);
    RBLNode *y = x->p;
    while (y != T->nil && x == y->right) {
        x = y;
        y = y->p;
    }
    if (y == T->nil)
        return RBLtreeMinimum(T, T->root);
    return RBLtreeMinimum(T, y->right);
}
RBLNode *RBLtreePredecessor(RBLTree *T, RBLNode *x) {
    if (x->left != T->nil)
        return RBLtreeMaximum(T, x->left);
    RBLNode *y = x->p;
    while (y != T->nil && x == y->left) {
        x = y;
        y = y->p;
    }
    if (y == T->nil)
        return RBLtreeMaximum(T, T->root);
    return RBLtreeMaximum(T, y->left);
}


// Common methods
RBLTree *RBLinit() {
    RBLNode *nil = malloc(sizeof(RBLNode));
    if (nil == NULL) return NULL;
    nil->p = NULL;
    nil->left = NULL;
    nil->right = NULL;
    nil->prev = NULL;
    nil->next = NULL;
    nil->color = BLACK;
    RBLTree *T = malloc(sizeof(RBLTree));
    if (T == NULL) return NULL;
    T->nil = nil;
    T->root = nil;
    return T;
}

RBLNode *RBLnewNode(int key, void *data) {
    RBLNode *x = malloc(sizeof(RBLNode));
    if (x == NULL) return NULL;
    x->key = key;
    x->data = data;
    return x;
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
    z->left = T->nil;
    z->right = T->nil;
    if (y == T->nil) {
        T->root = z;
        // Maintain list-pointers
        z->next = z;
        z->prev = z;
    } else {
        // Create new internal node with y's data
        RBLNode *u = RBLnewNode(y->key, NULL);
        if (y == T->root) {
            T->root = u;
            u->p = T->nil;
        } else {
            if (y->p->left == y)
                y->p->left = u;
            else {
                assert( y->p->right == y );
                y->p->right = u;
            }
            u->p = y->p;
        }
        y->p = u;
        z->p = u;

        if (z->key < y->key) {      // z shall be a left child
            u->left = z;
            u->right = y;
            u->key = z->key;
            // Maintain list-pointers
            z->prev = RBLtreePredecessor(T, z);
            z->prev->next = z;
            z->next = y;
            y->prev = z;
            y->next = RBLtreeSuccessor(T, y);
            y->next->prev = y;
        } else {                    // z shall be a right child
            u->left = y;
            u->right = z;
            // Maintain list-pointers
            y->prev = RBLtreePredecessor(T, y);
            y->prev->next = y;
            y->next = z;
            z->prev = y;
            z->next = RBLtreeSuccessor(T, z);
            z->next->prev = z;
        }
        u->color = y->color;
        y->color = RED;
    }
    z->color = RED;
    RBLinsertFixup(T, z);
}

void RBLdelete(RBLTree *T, RBLNode *z) {
    RBLNode *x;
    RBLNode *y = z;
    RBLColor yOriginalColor = y->color;
    if (z->left == T->nil) {
        x = z->right;
        RBLtransplant(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        RBLtransplant(T, z, z->left);
    } else {
        y = RBLtreeMinimum(T, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->p == z)
            x->p = y;
        else {
            RBLtransplant(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        RBLtransplant(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }
    if (yOriginalColor == BLACK)
        RBLdeleteFixup(T, x);
}


// Helper methods
void RBLleftRotate(RBLTree *T, RBLNode *x) {
    RBLNode *y = x->right;
    x->right = y->left;
    if (y->left != T->nil)
        x->right->p = x;
    y->p = x->p;
    if (x->p == T->nil)
        T->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}
void RBLrightRotate(RBLTree *T, RBLNode *y) {
    RBLNode *x = y->left;
    y->left = x->right;
    if (x->right != T->nil)
        y->left->p = y;
    x->p = y->p;
    if (y->p == T->nil)
        T->root = x;
    else if (y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;
    x->right = y;
    y->p = x;
}
void RBLinsertFixup(RBLTree *T, RBLNode *z) {
    RBLNode *y;
    while (z->p->color == RED) {
        if (z->p == z->p->p->left) {
            y = z->p->p->right;
            if (y->color == RED) {
                // case 1
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->right) {
                    // case 2
                    z = z->p;
                    RBLleftRotate(T, z);
                }
                // case 3
                z->p->color = BLACK;
                z->p->p->color = RED;
                RBLrightRotate(T, z->p->p);
            }
        } else {
            y = z->p->p->left;
            if (y->color == RED) {
                // case 1
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else {
                if (z == z->p->left) {
                    // case 2
                    z = z->p;
                    RBLrightRotate(T, z);
                }
                // case 3
                z->p->color = BLACK;
                z->p->p->color = RED;
                RBLleftRotate(T, z->p->p);
            }
        }
    }
    T->root->color = BLACK;
}
void RBLtransplant(RBLTree *T, RBLNode *u, RBLNode *v) {
    if (u->p == T->nil)
        T->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}
void RBLdeleteFixup(RBLTree *T, RBLNode *x) {
    while (x != T->root && x->color == BLACK) {
        if (x == x->p->left) {
            RBLNode *w = x->p->right;
            if (w->color == RED) {
                // case 1
                w->color = BLACK;
                x->p->color = RED;
                RBLleftRotate(T, x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                // case 2
                w->color = RED;
                x = x->p;
            } else {
                if (w->right->color == BLACK) {
                    // case 3
                    w->left->color = BLACK;
                    w->color = RED;
                    RBLrightRotate(T, w);
                    w = x->p->right;
                }
                // case 4
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                RBLleftRotate(T, x->p);
                x = T->root;
            }
        } else {
            RBLNode *w = x->p->left;
            if (w->color == RED) {
                // case 1
                w->color = BLACK;
                x->p->color = RED;
                RBLrightRotate(T, x->p);
                w = x->p->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                // case 2
                w->color = RED;
                x = x->p;
            } else {
                if (w->left->color == BLACK) {
                    // case 3
                    w->right->color = BLACK;
                    w->color = RED;
                    RBLleftRotate(T, w);
                    w = x->p->left;
                }
                // case 4
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                RBLrightRotate(T, x->p);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}


// testing methods
int RBLsubtreeHasOnlyBlackLeaves(RBLTree *T, RBLNode *x) {
    if (RBLisLeaf(T, x))
        return (x->color == BLACK);
    int left = 1, right = 1;
    if (RBLhasLeft(T, x))
        left = RBLsubtreeHasOnlyBlackLeaves(T, x->right);
    if (RBLhasRight(T, x))
        right = RBLsubtreeHasOnlyBlackLeaves(T, x->left);
    return left & right;
}
int RBLeachLeafIsBlack(RBLTree *T) {
    return RBLsubtreeHasOnlyBlackLeaves(T, T->root);
}
int RBLsubtreeRedNodeHasBlackChildren(RBLTree *T, RBLNode *x) {
    if (RBLisLeaf(T, x))
        return 1;
    int ok = 1;
    if (RBLhasColor(x, RED))
        ok = RBLhasColor(x->left, BLACK) & RBLhasColor(x->right, BLACK);
    return ok & RBLsubtreeRedNodeHasBlackChildren(T, x->left)
              & RBLsubtreeRedNodeHasBlackChildren(T, x->right);
}
int RBLeachRedNodeHasBlackChildren(RBLTree *T) {
    return RBLsubtreeRedNodeHasBlackChildren(T, T->root);
}
int RBLsubtreePathsHasEqualLength(RBLTree *T, RBLNode *x, int plen) {
    if (RBLisLeaf(T, x))
        return 1;
    int isBlack = RBLhasColor(x, BLACK),
        left = RBLsubtreePathsHasEqualLength(T, x->left, plen+isBlack),
        right = RBLsubtreePathsHasEqualLength(T, x->right, plen+isBlack);
    return left == right;
}
int RBLeachRootLeafPathHasEqualLength(RBLTree *T) {
    return RBLsubtreePathsHasEqualLength(T, T->root, 0);
}
int RBLisRBLTree(RBLTree *T) {
    int ok = (T->root->color == BLACK); // root should be black
    // Each leaf should be black
    ok &= RBLeachLeafIsBlack(T);
    // Each red node should have black children
    ok &= RBLeachRedNodeHasBlackChildren(T);
    // Each path from root to leaf shall have same # black nodes
    ok &= RBLeachRootLeafPathHasEqualLength(T);
    return ok;
}
int RBLisRBLTreeVerbose(RBLTree *T) {
    int ok = (T->root->color == BLACK); // root should be black
    if (ok) printf("  OK: the root is black\n");
    else    printf("  FAIL: the root is red\n");
    // Each leaf should be black
    ok &= RBLeachLeafIsBlack(T);
    if (ok) printf("  OK: Each leaf is black\n");
    else    printf("  FAIL: leaves are not black\n");
    // Each red node should have black children
    ok &= RBLeachRedNodeHasBlackChildren(T);
    if (ok) printf("  OK: Each red node has black children\n");
    else    printf("  FAIL: There exists a red node with a red child\n");
    // Each path from root to leaf shall have same # black nodes
    ok &= RBLeachRootLeafPathHasEqualLength(T);
    if (ok) printf("  OK: Each path from root to leaf has the same length\n");
    else    printf("  FAIL: Not all paths from root to leaf has same length\n");
    return ok;
}


// miscelanous
void RBLwriteNil(FILE *fd, RBLNode *x) {
    return;
    int id = nilnum++;
    fprintf(fd, "  nil%d [label=nil color=black shape=box width=0.25 height=0.25 fontsize=10];\n", id);
    fprintf(fd, "  n%p -> nil%d;\n", (void*)x, id);
}
void RBLwriteTreeNode(FILE *fd, RBLTree *T, RBLNode *x) {
    if (x != T->nil) {
        // Print node
        if (!RBLhasData(x)) {
            fprintf(fd, "  n%p [label=%d color=%s fontcolor=white];\n",
                (void*)x, x->key, (x->color==RED ? "firebrick":"black"));
        } else {
            fprintf(fd, "  n%p [shape=record label=\"{%d|{%d|%d}}\" color=%s fontcolor=yellow];\n",
                (void*)x, x->key, x->prev->key, x->next->key, (x->color==RED ? "firebrick":"black"));
        }
        // left
        if (RBLhasLeft(T, x))
            fprintf(fd, "  n%p -> n%p;\n", (void*)x, (void*)x->left);
        else
            RBLwriteNil(fd, x);
        // right
        if (RBLhasRight(T, x))
            fprintf(fd, "  n%p -> n%p;\n", (void*)x, (void*)x->right);
        else
            RBLwriteNil(fd, x);
        // List arrows
        // if (RBLhasData(x)) {
        //     if (x->prev != NULL && x->prev != x)
        //         fprintf(fd, "  n%p -> n%p [color=red constraint=false]\n", (void*)x, (void*)x->prev);
        //     if (x->next != NULL && x->next != x)
        //         fprintf(fd, "  n%p -> n%p [color=red constraint=false]\n", (void*)x, (void*)x->next);
        // }
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
    RBLwriteTreeNode(fd, T, T->root);
    fprintf(fd, "}\n");
    fclose(fd);
}

void RBLdestroy(RBLTree *T, RBLNode *x) {
    RBLdelete(T, x);
    free(x);
}

void RBLtreeDestroy(RBLTree *T) {
    while (!RBLisEmpty(T))
        RBLdestroy(T, T->root);
    free(T);
}