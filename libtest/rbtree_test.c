#include <stdio.h>
#include <stdlib.h>
#include "../lib/rbtree.h"

#define NODES 10

int main(int argc, char **argv) {
    printf("Creating empty RB tree\n");
    RBTree *tree = RBinit();

    // Insert a node
    int k = rand() % (NODES*10);
    printf("Inserting a node with key %d\n", k);
    RBinsert(tree, RBnewNode(k));
    // Insert #NODES nodes
    for (int i=0; i<NODES; i++) {
        k = rand() % (NODES*10+1);
        printf("Inserting a node with key %d\n", k);
        RBinsert(tree, RBnewNode(k));
    }
    // Legal RB tree?
    printf("Is the tree a RedBlack search tree?\n");
    printf("  => %s\n", (RBisRBTree(tree) ? "YES" : "NO"));
    // Print tree
    RBwriteTree(tree, "tree.dot");
    // return 0;
    // Remove all nodes
    printf("Removing all nodes in the tree:\n");
    RBNode *x;
    int n;
    for (n=0; !RBisEmpty(tree); n++) {
        x = RBtreeMinimum(tree, tree->root);
        printf("  removing node with key %d\n", x->key);
        RBdelete(tree, x);
        free(x);
    }
    printf("Removed %d nodes\n", n);
    free(tree);

    return 0;
}