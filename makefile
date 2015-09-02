CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lSDL2 -lm
OBJS = convexhull.o
PROG = convexhull
CXX = gcc

%.o: %.c %.h
	$(CXX) $(CFLAGS) -c -s $<

%.out: %.o
	$(CXX) $< -o $@ $(LFLAGS)

$(PROG): $(OBJS)
	$(CXX) $(OBJS) -o $(PROG) $(LFLAGS)

# Phony targets
.PHONY: clean
clean:
	rm -f *.o *.out $(PROG)

disinfect:
	rm -f *.o *.out