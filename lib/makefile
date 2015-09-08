CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lm
CXX = gcc

all: rbtree.o

%.o: %.c %.h
	$(CXX) -c $<

# Phony targets
.PHONY: clean
clean:
	rm -f *.o