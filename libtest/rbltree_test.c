#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../lib/rbltree.h"

#define TB "\033[1m"
#define TG "\033[32m"
#define TR "\033[31m"
#define TE "\033[0m"

#define NUM_TESTS_VERBOSE 1
#define NUM_TESTS_NORMAL 7
#define NODES_DEFAULT 25

int USE_COLORS = 1;

#define THEAD(desc) do {                                        \
    if (USE_COLORS)                                             \
        printf("\ttest: " TB "%-40s" TE "\n", desc);            \
    else                                                        \
        printf("\ttest: %-40s\n", desc);                        \
} while (0)
#define TFOOT(ok) do {                                          \
    if (USE_COLORS) {                                           \
        if (ok) printf("\t      => " TG "SUCCESS" TE "\n");     \
        else    printf("\t      => " TR "FAIL" TE "\n");        \
    } else                                                      \
        printf("\t      => %s\n", (ok?"SUCCESS":"FAIL"));       \
} while (0)
#define TERROR(ok, message) do {                                \
    if (!ok) printf("ERROR (%d): " message, __LINE__);          \
} while (0)
#define TERRORARG(ok, message, ...) do {                        \
    if (!ok)printf("ERROR (%d): " message,__LINE__,__VA_ARGS__);\
} while (0)


// Utility functions
int arrayMin(int *keys, int n) {
    int min = keys[0];
    for (int i=1; i<n; i++)
        min = (min < keys[i] ? min : keys[i]);
    return min;
}
int arrayMax(int *keys, int n) {
    int max = keys[0];
    for (int i=1; i<n; i++)
        max = (max > keys[i] ? max : keys[i]);
    return max;
}


/**
 * Verbose test (with printing of tree) of most parts of implementation:
 *     insertion, deletion, istreetest, destroy, minimum, ...
 * Success: if tree is an rbl-tree after every operation
 */
int test_verbose(int nodes) {
    THEAD("Verbose run-test of insert+delete");

    int ok = 1;
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
        ok &= RBLisRBLTree(tree);
        if (!ok) printf("=> FAIL: is not RBL-tree\n");
    }
    // Legal RBL tree?
    printf("Is the tree a RedBlack search tree?\n");
    printf("  => %s\n", (RBLisRBLTreeVerbose(tree) ? "YES" : "NO"));
    // Print final tree
    snprintf(fname, 25, "rbltree_%05d.dot", i);
    RBLwriteTree(tree, fname);
    // Remove all nodes
    printf("Removing all nodes in the tree:\n");
    RBLNode *x;
    int n;
    for (n=i+1; !RBLisEmpty(tree); i++) {
        snprintf(fname, 25, "rbltree_%05d.dot", i);
        RBLwriteTree(tree, fname);
        x = RBLtreeMinimum(tree, tree->root);
        printf("  removing node with key %d\n", x->key);
        RBLdestroy(tree, x);
        ok &= RBLisRBLTree(tree);
        if (!ok) printf("=> FAIL: is not RBL-tree\n");
    }
    printf("Removed %d nodes\n", n);
    RBLtreeDestroy(tree);

    TFOOT(ok);
    return ok;
}

/**
 * Tests insertion of nodes random elements.
 * Success: if it is RBL tree afterwards
 */
int test_insertRandom(int nodes) {
    THEAD("Insertion of nodes");

    int ok = 1;
    RBLTree *tree = RBLinit();
    int i, k;
    for (i=0; i<nodes; i++) {
        k = rand() % (nodes*nodes+1);
        RBLinsert(tree, RBLnewNode(k, &k));
        ok &= RBLisRBLTree(tree);
    }
    RBLtreeDestroy(tree); // Not part of test
    
    TFOOT(ok);
    return ok;
}

/**
 * Tests RBLtreeSearch: for a given tree and an array of the values in the tree,
 * it runs through each key-value and checks that there is a node with the key.
 */
int test_search(RBLTree *tree, int *keys, int n) {
    THEAD("Tree-search");

    int ok = 1;
    RBLNode *x;
    for (int i=0; i<n; i++) {
        x = RBLtreeSearch(tree, keys[i]);
        ok &= (x != tree->nil && x->key == keys[i]);
    }

    TFOOT(ok);
    return ok;
}

/**
 * Tests RBLtreeSearch (iterative version)
 */
int test_searchIterative(RBLTree *tree, int *keys, int n) {
    THEAD("Tree-search (iterative)");

    int ok = 1;
    RBLNode *x;
    for (int i=0; i<n; i++) {
        x = RBLtreeSearchIterative(tree, keys[i]);
        ok &= (x != tree->nil && x->key == keys[i]);
    }

    TFOOT(ok);
    return ok;
}

/**
 * Find minimum element in a search tree
 */
int test_minimum(RBLTree *tree, int *keys, int n) {
    THEAD("Tree-minimum");

    int ok, min = arrayMin(keys, n);
    RBLNode *x;
    if (n > 0) {
        ok = ((x=RBLtreeMinimum(tree, tree->root))->key == min);
        TERRORARG(ok, "Expected key %d, got %d\n", min, x->key);
    } else {
        ok = ((x=RBLtreeMinimum(tree, tree->root)) == tree->nil);
        TERROR(ok, "Expected nil-node\n");
    }

    TFOOT(ok);
    return ok;
}

/**
 * Find maximum element in a search tree
 */
int test_maximum(RBLTree *tree, int *keys, int n) {
    THEAD("Tree-maximum");

    int ok, max = arrayMax(keys, n);
    RBLNode *x;
    if (n > 0) {
        ok = ((x=RBLtreeMaximum(tree, tree->root))->key == max);
        TERRORARG(ok, "Expected key %d, got %d\n", max, x->key);
    } else {
        ok = ((x=RBLtreeMaximum(tree, tree->root)) == tree->nil);
        TERROR(ok, "Expected nil-node\n");
    }

    TFOOT(ok);
    return ok;
}

/**
 * Find successor element
 */
int test_successor(RBLTree *tree, int *keys, int n) {
    THEAD("Tree-successor");

    int ok = 1;
    RBLNode *x = RBLtreeMinimum(tree, tree->root),
            *y;
    for (int i=0; i<n-1; i++) {
        y = RBLtreeSuccessor(tree, x);
        ok &= (y->key >= x->key);
        TERRORARG(ok, "Expected y{%d} >= x{%d} (i=%d)\n", y->key, x->key, i);
        x = y;
    }

    TFOOT(ok);
    return ok;
}

/**
 * Find predecessor element
 */
int test_predecessor(RBLTree *tree, int *keys, int n) {
    THEAD("Tree-predecessor");

    int ok = 1;
    RBLNode *x = RBLtreeMaximum(tree, tree->root),
            *y;
    for (int i=0; i<n-1; i++) {
        y = RBLtreePredecessor(tree, x);
        ok &= (y->key <= x->key);
        TERRORARG(ok, "Expected y{%d} <= x{%d} (i=%d)\n", y->key, x->key, i);
        x = y;
    }

    TFOOT(ok);
    return ok;
}


void prepare_tests(RBLTree *tree, int *keys, int n) {
    RBLNode *x;
    for (int i=0; i<n; i++) {
        keys[i] = rand() % (n*n);
        x = RBLnewNode(keys[i], &keys[i]);
        RBLinsert(tree, x);
    }
}
void cleanup_tests(RBLTree *tree, int *keys, int n) {
    RBLtreeDestroy(tree);
    free(keys);
}

/**
 * RUN ALL TESTS
 */
int main(int argc, char **argv) {
    int testi = 0;
    int N = NODES_DEFAULT,
        verbose = 0;
    if (argc >= 2)
        N = atoi(argv[1]);
    if (argc >= 3)
        verbose = (strcmp(argv[2],"-v")==0 || !strcmp(argv[2],"-nv")==0);
    if (argc >= 4)
        USE_COLORS = (strcmp(argv[3],"-c")==0 || !strcmp(argv[3],"-nc")==0);
    printf("Testing with N=%d...\n", N);
    if (verbose)
        printf("Enabled: Verbose test.\n");
    if (!USE_COLORS)
        printf("Disabled: fancy colors for output.\n");

    int *tests;
    printf("\n===============================\n");
    printf("Testing:\n");
    if (verbose) {
        tests = calloc(NUM_TESTS_VERBOSE, sizeof(int));
        // Verbose tests here
        tests[testi++] = test_verbose(N);

        // Done
        assert(testi == NUM_TESTS_VERBOSE);
    } else {
        // Prepare some tests:
        RBLTree *tree = RBLinit();
        int *keys = calloc(N, sizeof(int));
        prepare_tests(tree, keys, N);

        tests = calloc(NUM_TESTS_NORMAL, sizeof(int));
        // All normal tests here
        tests[testi++] = test_insertRandom(N);
        tests[testi++] = test_search(tree, keys, N);
        tests[testi++] = test_searchIterative(tree, keys, N);
        tests[testi++] = test_minimum(tree, keys, N);
        tests[testi++] = test_minimum(tree, keys, N);
        tests[testi++] = test_successor(tree, keys, N);
        tests[testi++] = test_predecessor(tree, keys, N);

        // Done
        cleanup_tests(tree, keys, N);
        assert(testi == NUM_TESTS_NORMAL);
    }
    int failures=0, succeses=0;
    int success;
    for (int i=0; i<testi; i++) {
        success = tests[i];
        failures += !success;
        succeses += success;
    }

    printf("===============================\n");
    printf("Performed %d tests:\n", testi);
    printf("\t%3d failures\n", failures);
    printf("\t%3d succeses\n", succeses);

    return (failures != 0); // returns 0 if no failures, 1 if at least one
}