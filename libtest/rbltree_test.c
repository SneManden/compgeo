#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../lib/rbltree.h"

// bold, Red, Green, Yellow, Blue, End
#define Tb "\033[1m"
#define TR "\033[31m"
#define TG "\033[32m"
#define TY "\033[33m"
#define TB "\033[34m"
#define TE "\033[0m"

#define NUM_TESTS_VERBOSE 1
#define NUM_TESTS_NORMAL 8
#define NODES_DEFAULT 25
#define RUNSMAX 100

int USE_COLORS = 1;

#define THEAD(desc) do {                                            \
    if (USE_COLORS)                                                 \
        printf("\ttest: " Tb "%-40s" TE, desc);                     \
    else                                                            \
        printf("\ttest: %-40s", desc);                              \
} while (0)
#define TFOOT(ok) do {                                              \
    if (USE_COLORS) {                                               \
        if (ok==1)      printf("\t      => " TG "SUCCESS" TE "\n"); \
        else if (ok==0) printf("\t      => " TR "FAIL" TE "\n");    \
        else            printf("\t      => " TY "MANUAL" TE "\n");  \
    } else                                                          \
        printf("\t      => %s\n", (ok?"SUCCESS":"FAIL"));           \
} while (0)
#define TERROR(ok, message) do {                                    \
    if (!ok) printf("ERROR (%d): " message, __LINE__);              \
} while (0)
#define TERRORARG(ok, message, ...) do {                            \
    if (!ok)printf("ERROR (%d): " message,__LINE__,__VA_ARGS__);    \
} while (0)


// Utility functions
int arrayMin(int *keys, int n) {
    if (n == 0) return -1;
    int min = keys[0];
    for (int i=1; i<n; i++)
        min = (min < keys[i] ? min : keys[i]);
    return min;
}
int arrayMax(int *keys, int n) {
    if (n == 0) return -1;
    int max = keys[0];
    for (int i=1; i<n; i++)
        max = (max > keys[i] ? max : keys[i]);
    return max;
}
// http://stackoverflow.com/a/5249150
#define getTimeDiff(te, ts) ((double)(te - ts) / CLOCKS_PER_SEC)
#define setTime(t) do { t = clock(); } while (0)
#define printExecTime(tr, tp) \
    printf("\tExecution time (sec): %.5f (prep: %.5f, total: %.5f)\n", tr, tp, tr+tp)

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
    RBLNode *x = RBLtreeMinimum(tree, tree->root);
    if (n > 0) {
        ok = (x->key == min);
        TERRORARG(ok, "Expected key %d, got %d\n", min, x->key);
    } else {
        ok = (x == tree->nil);
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
    RBLNode *x = RBLtreeMaximum(tree, tree->root);
    if (n > 0) {
        ok = (x->key == max);
        TERRORARG(ok, "Expected key %d, got %d\n", max, x->key);
    } else {
        ok = (x == tree->nil);
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

/**
 * Tests that each leaf is connected to its leaf-predecessor and leaf-successor,
 * including the minimum and maximum elements being connected
 */
int test_linkedList(RBLTree *tree, int *keys, int n) {
    THEAD("Linked-list");

    int ok = 1;
    if (n == 0) {
        TFOOT(ok);
        return ok;
    }

    RBLNode *x = RBLtreeMinimum(tree, tree->root);
    RBLNode *y = RBLtreeMaximum(tree, tree->root);
    ok &= (x==y->next && y==x->prev);
    TERRORARG(ok, "Expected min x{%d} and max y{%d} to be connected\n", x->key, y->key);
    for (int i=0; i<n-1; i++) {
        y = RBLtreeSuccessor(tree, x);
        ok &= (x==y->prev && y==x->next);
        TERRORARG(ok, "Expected x{%d} and y{%d} to be connected\n", x->key, y->key);
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
    printf("Set: N=%d.\n", N);
    if (verbose)
        printf("Enabled: Verbose test.\n");
    if (!USE_COLORS)
        printf("Disabled: fancy colors for output.\n");

    int *tests;
    printf("===============================\n");
    printf("Testing:\n");
    int runs = 0;
    if (verbose) {
        tests = calloc(NUM_TESTS_VERBOSE, sizeof(int));
        // Verbose tests here
        tests[0] = test_verbose(N);
        testi = NUM_TESTS_VERBOSE;
    } else {
        tests = calloc(NUM_TESTS_NORMAL, sizeof(int));
        RBLTree *tree = NULL;
        int *keys = NULL;
        int M = 0,
            j = 0;
        clock_t t_start, t_end, t_prepstart, t_prepend;
        double t_run, t_prep;
        for (int i=0; i<RUNSMAX; i++) {
            printf("    Tree-size %d:\n", M);
            // Prepare tests:
            keys = calloc(M, sizeof(int));
            setTime(t_prepstart);
            tree = RBLinit();
            prepare_tests(tree, keys, M);
            setTime(t_prepend);
            t_prep = getTimeDiff(t_prepend, t_prepstart);
            // All normal tests here
            setTime(t_start);
            tests[(j++)%NUM_TESTS_NORMAL] += test_insertRandom(M);
            tests[(j++)%NUM_TESTS_NORMAL] += test_search(tree, keys, M);
            tests[(j++)%NUM_TESTS_NORMAL] += test_searchIterative(tree, keys, M);
            tests[(j++)%NUM_TESTS_NORMAL] += test_minimum(tree, keys, M);
            tests[(j++)%NUM_TESTS_NORMAL] += test_minimum(tree, keys, M);
            tests[(j++)%NUM_TESTS_NORMAL] += test_successor(tree, keys, M);
            tests[(j++)%NUM_TESTS_NORMAL] += test_predecessor(tree, keys, M);
            tests[(j++)%NUM_TESTS_NORMAL] += test_linkedList(tree, keys, M);
            setTime(t_end);
            t_run = getTimeDiff(t_end, t_start);
            printExecTime(t_run, t_prep);
            runs++;
            j = 0;
            // Done
            cleanup_tests(tree, keys, M);
            M = (M==0 ? 1 : 2*M);
            if (M > N)
                break;
        }
        testi = NUM_TESTS_NORMAL;
    }
    int failures=0, succeses=0;
    int success;
    for (int i=0; i<testi; i++) {
        success = tests[i];
        failures += (runs - success);
        succeses += success;
    }

    printf("===============================\n");
    printf("Performed %d tests:\n", testi);
    printf("\t%3d failures\n", failures);
    printf("\t%3d succeses\n", succeses);

    return (failures != 0); // returns 0 if no failures, 1 if at least one
}