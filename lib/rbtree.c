#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h> // malloc
#include <assert.h>

// Forward references
void RBleftRotate(RBTree *T, RBNode *x);
void RBrightRotate(RBTree *T, RBNode *x);
void RBinsertFixup(RBTree *T, RBNode *z);
void RBtransplant(RBTree *T, RBNode *u, RBNode *v);
void RBdeleteFixup(RBTree *T, RBNode *x);

static nilnum = 0;

RBTree *RBinit() {
    RBNode *nil = malloc(sizeof(RBNode));
    if (nil == NULL) return NULL;
    nil->p = NULL;
    nil->left = NULL;
    nil->right = NULL;
    nil->color = BLACK;
    RBTree *T = malloc(sizeof(RBTree));
    if (T == NULL) return NULL;
    T->nil = nil;
    T->root = nil;
    return T;
}

RBNode *RBnewNode(int key) {
    RBNode *x = malloc(sizeof(RBNode));
    if (x == NULL) return NULL;
    x->key = key;
    return x;
}

RBNode *RBtreeSearch(RBTree *T, RBNode *x, int k) {
    if (x == T->nil || k == x->key)
        return x;
    if (k < x->key)
        return RBtreeSearch(T, x->left, k);
    else
        return RBtreeSearch(T, x->right, k);
}

RBNode *RBtreeSearchIterative(RBTree *T, RBNode *x, int k) {
    while (x != T->nil && k != x->key) {
        if (k < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

RBNode *RBtreeMinimum(RBTree *T, RBNode *x) {
    while (x->left != T->nil)
        x = x->left;
    return x;
}

RBNode *RBtreeMaximum(RBTree *T, RBNode *x) {
    while (x->right != T->nil)
        x = x->right;
    return x;
}

RBNode *RBtreeSuccessor(RBTree *T, RBNode *x) {
    if (x->right != T->nil)
        return RBtreeMinimum(T, x->right);
    RBNode *y = x->p;
    while (y != T->nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

RBNode *RBtreePredecessor(RBTree *T, RBNode *x) {
    if (x->left != T->nil)
        return RBtreeMaximum(T, x->left);
    RBNode *y = x->p;
    while (y != T->nil && x == y->left) {
        x = y;
        y = y->p;
    }
    return y;
}

void RBleftRotate(RBTree *T, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (x->right != T->nil)
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

    assert( T->nil->color != RED );
}

void RBrightRotate(RBTree *T, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != T->nil)
        x->right->p = y;
    x->p = y->p;
    if (x->p == T->nil)
        T->root = x;
    else if (y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;
    x->right = y;
    y->p = x;

    assert( T->nil->color != RED );
}

void RBinsert(RBTree *T, RBNode *z) {
    RBNode *y = T->nil;
    RBNode *x = T->root;
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
    else if (z->key < y->key)
        y->left= z;
    else
        y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    RBinsertFixup(T, z);
}

void RBinsertFixup(RBTree *T, RBNode *z) {
    RBNode *y;
    while (z->p->color == RED) {
        if (z->p == z->p->p->left) {
            y = z->p->p->right;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else if (z == z->p->right) {
                z = z->p;
                RBleftRotate(T, z);
                z->p->color = BLACK;
                z->p->p->color = RED;
                RBrightRotate(T, z->p->p);
            }
        } else {
            y = z->p->p->left;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            } else if (z == z->p->left) {
                z = z->p;
                RBleftRotate(T, z);
                z->p->color = BLACK;
                z->p->p->color = RED;
                RBrightRotate(T, z->p->p);
            }
        }
    }
    T->root->color = BLACK;
}

void RBtransplant(RBTree *T, RBNode *u, RBNode *v) {
    if (u->p == T->nil)
        T->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

void RBdelete(RBTree *T, RBNode *z) {
    RBNode *x;
    RBNode *y = z;
    RBColor yOriginalColor = y->color;
    if (z->left == T->nil) {
        x = z->right;
        RBtransplant(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        RBtransplant(T, z, z->left);
    } else {
        y = RBtreeMinimum(T, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->p == z)
            x->p = y;
        else {
            RBtransplant(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        RBtransplant(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }
    if (yOriginalColor == BLACK)
        RBdeleteFixup(T, x);
}

void RBdeleteFixup(RBTree *T, RBNode *x) {
    while (x != T->root && x->color == BLACK) {
        if (x == x->p->left) {
            RBNode *w = x->p->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                RBleftRotate(T, x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            } else if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                RBrightRotate(T, w);
                w = x->p->right;
                // case 4
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                RBleftRotate(T, x->p);
                x = T->root;
            }
        } else {
            RBNode *w = x->p->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                RBrightRotate(T, x->p);
                w = x->p->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->p;
            } else if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                RBleftRotate(T, w);
                w = x->p->left;
                // case 4
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                RBrightRotate(T, x->p);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

int RBsubtreeHasOnlyBlackLeaves(RBTree *T, RBNode *x) {
    if (RBisLeaf(T, x))
        return (x->color == BLACK);
    int left = 1, right = 1;
    if (RBhasLeft(T, x))
        left = RBsubtreeHasOnlyBlackLeaves(T, x->right);
    if (RBhasRight(T, x))
        right = RBsubtreeHasOnlyBlackLeaves(T, x->left);
    return left & right;
}
int RBeachLeafIsBlack(RBTree *T) {
    return RBsubtreeHasOnlyBlackLeaves(T, T->root);
}

// Assumes red nodes has children (i.e. RBeachLeafIsBlack == true)
int RBsubtreeRedNodeHasBlackChildren(RBTree *T, RBNode *x) {
    if (RBisLeaf(T, x))
        return 1;
    int ok = 1;
    if (RBhasColor(x, RED))
        ok = RBhasColor(x->left, BLACK) & RBhasColor(x->right, BLACK);
    return ok & RBsubtreeRedNodeHasBlackChildren(T, x->left)
              & RBsubtreeRedNodeHasBlackChildren(T, x->right);
}
int RBeachRedNodeHasBlackChildren(RBTree *T) {
    return RBsubtreeRedNodeHasBlackChildren(T, T->root);
}

// Assumes leaves are black (i.e. RBeachLeafIsBlack == true)
int RBsubtreePathsHasEqualLength(RBTree *T, RBNode *x, int plen) {
    if (RBisLeaf(T, x))
        return plen+1;
    int isBlack = RBhasColor(x, BLACK),
        left = plen, right = plen;
    if (RBhasLeft(T, x))
        left = RBsubtreePathsHasEqualLength(T, x->left, plen+isBlack);
    if (RBhasRight(T, x))
        right = RBsubtreePathsHasEqualLength(T, x->right, plen+isBlack);
    return left == right;
}
int RBeachRootLeafPathHasEqualLength(RBTree *T) {
    return RBsubtreePathsHasEqualLength(T, T->root, 0);
}

int RBisRBTree(RBTree *T) {
    int ok = (T->root->color == BLACK); // root should be black
    printf("  (the root is black)\n");
    // Each leaf should be black
    ok &= RBeachLeafIsBlack(T);
    printf("  (Each leaf is black)\n");
    // Each red node should have black children
    ok &= RBeachRedNodeHasBlackChildren(T);
    printf("  (Each red node has black children)\n");
    // Each path from root to leaf shall have same # black nodes
    ok &= RBeachRootLeafPathHasEqualLength(T);
    printf("  (Each path from root to leaf has the same length)\n");
    return ok;
}

void RBwriteNil(FILE *fd, RBNode *x) {
    int id = nilnum++;
    //fprintf(fd, "  n%d [label=nil color=black shape=box]\n", id);
    //fprintf(fd, "  n%d -> n%d [label=nil color=black]\n", x->key, id);
}
void RBwriteTreeNode(FILE *fd, RBTree *T, RBNode *x) {
    if (x != T->nil) {
        // Print node
        fprintf(fd, "  n%d [label=%d color=%s];\n",
            x->key, x->key, (x->color==RED ? "firebrick":"black"));
        if (RBhasLeft(T, x))
             fprintf(fd, "  n%d -> n%d\n", x->key, x->left->key);
        else RBwriteNil(fd, x);
        if (RBhasRight(T, x))
             fprintf(fd, "  n%d -> n%d\n", x->key, x->right->key);
        else RBwriteNil(fd, x);
        RBwriteTreeNode(fd, T, x->left);
        RBwriteTreeNode(fd, T, x->right);
    }
}
void RBwriteTree(RBTree *T, char *filename) {
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