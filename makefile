PROG = convexhull linsegintersect
# SFML and C++
CPPFLAGS = -Wall
LPPFLAGS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system
# SDL and C
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lSDL2 -lm

# SFML and C++
%: %.cpp
	g++ $(CPPFLAGS) -c $@.cpp
	g++ -o $@ $@.o $(LPPFLAGS)

# SDL and C
%: %.c
	gcc $(CFLAGS) -c $@.c
	gcc -o $@ $@.o $(LFLAGS)

# Phony targets
.PHONY: clean
clean:
	rm -f *.o *.out $(PROG)

disinfect:
	rm -f *.o *.out