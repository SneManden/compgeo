PROG = rbtree_test
# SFML and C++
CPPFLAGS = -Wall
LPPFLAGS = -lm
# SDL and C
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lm

all: $(PROG)

# SDL and C
rbtree_heads = ../lib/rbtree.h
rbtree_deps = rbtree_test.o ../lib/rbtree.o
%.o: %.c $(rbtree_heads)
	gcc $(CFLAGS) -c $<
rbtree_test: $(rbtree_deps)
	gcc -o $@ $(rbtree_deps) $(LFLAGS)

# Phony targets
.PHONY: clean
clean:
	rm -f *.o $(PROG)

disinfect:
	rm -f *.o