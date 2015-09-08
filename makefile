OBJS = convexhull.o linsegintersect.o
# C++
CPPFLAGS = -Wall
LPPFLAGS = -lm
# C
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lm

all: $(OBJS)

# C++
%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

# C
%.o: %.c
	gcc $(CFLAGS) -c $<

# Phony targets
.PHONY: clean
clean:
	rm -f *.o

disinfect:
	rm -f *.o