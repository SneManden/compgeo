PROG = rbtree_test
# SFML and C++
CPPFLAGS = -Wall
LPPFLAGS = -lm
# SDL and C
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lm
# dot/png files
DOTFILES = $(wildcard *.dot)
PNGFILES = $(patsubst %.dot,png/%.png,$(DOTFILES))

all: $(PROG)

# SDL and C
rbtree_heads = ../lib/rbtree.h
rbtree_deps = rbtree_test.o ../lib/rbtree.o
%.o: %.c $(rbtree_heads)
	gcc $(CFLAGS) -c $<
rbtree_test: $(rbtree_deps)
	gcc -o $@ $(rbtree_deps) $(LFLAGS)

png/%.png: %.dot tree.gv pngdir
	dot $*.dot | gvpr -c -ftree.gv | neato -n -Tpng -o png/$*.png

pngdir:
	mkdir -p png

# Phony targets
.PHONY: clean
resizepng: pngdir $(PNGFILES)
	mogrify -background white -gravity center -extent 1920x540 png/*.png
	mogrify -resize 960x600 png/*.png
treegif: resizepng
	convert -delay 100 -loop 0 png/*.png tree.gif

clean:
	rm -rf *.o $(PROG) $(DOTFILES) png *.gif