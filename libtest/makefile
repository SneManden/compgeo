PROG = rbtree_test rbltree_test
# SFML and C++
CPPFLAGS = -Wall
LPPFLAGS = -lm
# SDL and C
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lm
# dot/png files
DOTFILES = $(wildcard *.dot)
PNGFILES = $(patsubst %.dot,png/%.png,$(DOTFILES))
# lib files
rbtree_heads = ../lib/rbtree.h
rbtree_deps = ../lib/rbtree.o
rbltree_heads = ../lib/rbltree.h
rbltree_deps = ../lib/rbltree.o

all: $(PROG)

# SDL and C
../lib/rbtree.o: ../lib/rbtree.h ../lib/rbtree.c
	make -C ../lib ../lib/rbtree.o
../lib/rbltree.o: ../lib/rbltree.h ../lib/rbltree.c
	make -C ../lib ../lib/rbltree.o

%.o: %.c $(rbtree_heads)
	gcc $(CFLAGS) -c $<
rbtree_test: rbtree_test.o $(rbtree_deps)
	gcc -o $@ $@.o $(rbtree_deps) $(LFLAGS)
rbltree_test: rbltree_test.o $(rbltree_deps) $(rbtree_deps)
	gcc -o $@ $@.o $(rbltree_deps) $(rbtree_deps) $(LFLAGS)

png/%.png: %.dot tree.gv pngdir
	dot $*.dot | gvpr -c -ftree.gv | neato -n -Tpng -o png/$*.png

pngdir:
	mkdir -p png

# Phony targets
.PHONY: clean
pngimages: $(DOTFILES) $(PNGFILES)

resizepng: pngdir $(PNGFILES)
	mogrify -background white -gravity center -extent 1920x540 png/*.png
	mogrify -resize 960x600 png/*.png
treegif: resizepng
	convert -delay 100 -loop 0 png/*.png tree.gif

clean:
	rm -rf *.o $(PROG) $(DOTFILES) png *.gif