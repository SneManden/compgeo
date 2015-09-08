PROG = linsegintersect
# SFML and C++
CPPFLAGS = -Wall -lm
LPPFLAGS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system
# SDL and C
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -lSDL2 -lm

all: $(PROG)

# SFML and C++
%: %.cpp
	g++ $(CPPFLAGS) -c $@.cpp
	g++ -o $@ $@.o ../$@.o $(LPPFLAGS)

# SDL and C
%: %.c
	gcc $(CFLAGS) -c $@.c
	gcc -o $@ $@.o ../$@.o $(LFLAGS)

# Phony targets
.PHONY: clean
clean:
	rm -f *.o $(PROG)

disinfect:
	rm -f *.o