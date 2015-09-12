#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../lib/rbltree.h"

#define NUM_TESTS_VERBOSE 1
#define NUM_TESTS_NORMAL 1
#define NODES_DEFAULT 25

#define test_header(desc) printf("\ttest: '%s'\n", desc)
#define test_footer(ok)   printf("\t      => %s\n", (ok?"SUCCESS":"FAIL"))

/**
 * Verbose test (with printing of tree) of most parts of implementation:
 *     insertion, deletion, istreetest, destroy, minimum, ...
 * Success: if tree is an rbl-tree after every operation
 */
int test_verbose(int nodes) {
    test_header("Verbose run-test of insert+delete");

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

    test_footer(ok);
    return ok;
}

/**
 * Tests insertion of nodes random elements.
 * Success: if it is RBL tree afterwards
 */
int test_insertRandom(int nodes) {
    test_header("Insertion of nodes");

    int ok = 1;
    RBLTree *tree = RBLinit();
    int i, k;
    for (i=0; i<nodes; i++) {
        k = rand() % (nodes*nodes+1);
        RBLinsert(tree, RBLnewNode(k, &k));
        ok &= RBLisRBLTree(tree);
    }
    RBLtreeDestroy(tree); // Not part of test
    
    test_footer(ok);
    return ok;
}

/**
 * Tests RBLtreeSearch: for a given tree and an array of the values in the tree,
 * it runs through each key-value and checks that there is a node with the key.
 */
int test_search(RBLTree *tree, int *keys, int n) {
    test_header("Tree-search");
    assert(tree != NULL);
    assert(keys != NULL);

    int ok = 1;
    RBLNode *x;
    for (int i=0; i<n; i++) {
        x = RBLtreeSearch(tree, keys[i]);
        ok &= (x != tree->nil);
    }

    test_footer(ok);
    return ok;
}

/**
 * Tests RBLtreeSearch (iterative version)
 */
int test_searchIterative(RBLTree *tree, int *keys, int n) {
    test_header("Tree-search (iterative)");

    int ok = 1;
    RBLNode *x;
    for (int i=0; i<n; i++) {
        x = RBLtreeSearchIterative(tree, keys[i]);
        ok &= (x != tree->nil);
    }

    test_footer(ok);
    return ok;
}

void prepare_tests(RBLTree **tree, int **keys, int n) {
    *tree = RBLinit();
    *keys = calloc(n, sizeof(int));
    for (int i=0; i<n; i++) {
        *keys[i] = rand() % (n*n);
        RBLinsert(*tree, RBLnewNode(*keys[i], &*keys[i]));
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
        verbose = (strcmp(argv[2], "-v") == 0);
    printf("Testing with N=%d...\n", N);
    if (verbose)
        printf("Verbose test activated.\n");

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
        RBLTree *tree = NULL;
        int *keys = NULL;
        prepare_tests(&tree, &keys, N);

        tests = calloc(NUM_TESTS_NORMAL, sizeof(int));
        // All normal tests here
        tests[testi++] = test_insertRandom(N);
        tests[testi++] = test_search(tree, keys, N);
        tests[testi++] = test_searchIterative(tree, keys, N);

        cleanup_tests(tree, keys, N);

        // Done
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