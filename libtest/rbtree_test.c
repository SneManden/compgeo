#include <stdio.h>
#include <stdlib.h>
#include "../lib/rbtree.h"

#define NODES_DEFAULT 25

int main(int argc, char **argv) {
    int nodes = NODES_DEFAULT;
    if (argc == 2)
        nodes = atoi(argv[1]);
    printf("Creating %d nodes...\n", nodes);

    printf("Creating empty RB tree\n");
    RBTree *tree = RBinit();

    int i, k;
    char *fname = malloc(sizeof(char)*22);
    // Insert #nodes nodes
    for (i=0; i<nodes; i++) {
        snprintf(fname, 22, "tree_%05d.dot", i);
        RBwriteTree(tree, fname);

        k = rand() % (nodes*10+13);
        printf("Inserting a node with key %d\n", k);
        RBinsert(tree, RBnewNode(k, &k));
    }
    // Legal RB tree?
    printf("Is the tree a RedBlack search tree?\n");
    printf("  => %s\n", (RBisRBTree(tree) ? "YES" : "NO"));
    // Print final tree
    snprintf(fname, 22, "tree_%05d.dot", i);
    RBwriteTree(tree, fname);
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