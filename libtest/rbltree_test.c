#include <stdio.h>
#include <stdlib.h>
#include "../lib/rbltree.h"

#define NODES_DEFAULT 25

int main(int argc, char **argv) {
    int nodes = NODES_DEFAULT;
    if (argc == 2)
        nodes = atoi(argv[1]);
    printf("Creating %d nodes...\n", nodes);

    printf("Creating empty RBL tree\n");
    RBLTree *tree = RBLinit();

    int i, k;
    char *fname = malloc(sizeof(char)*25);
    // Insert #nodes nodes
    for (i=0; i<nodes; i++) {
        snprintf(fname, 25, "rbltree_%05d.dot", i);
        RBLwriteTree(tree, fname);

        k = rand() % (nodes*10+13);
        printf("Inserting a node with key %d\n", k);
        RBLinsert(tree, RBLnewNode(k, &k));
    }
    // Legal RBL tree?
    printf("Is the tree a RedBlack search tree?\n");
    printf("  => %s\n", (RBLisRBLTree(tree) ? "YES" : "NO"));
    // Print final tree
    snprintf(fname, 25, "rbltree_%05d.dot", i);
    RBLwriteTree(tree, fname);
    
    free(tree);

    return 0;
}