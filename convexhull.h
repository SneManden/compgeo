#ifndef __convexhull_h
#define __convexhull_h

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
// Using BSD queue.h for datastructures
// http://stackoverflow.com/a/668567
#include "lib/queue.h"

#define true 1
#define false 0

// Extends CIRCLEQ
#define CIRCLEQ_NEW(name, variable) struct name *variable = malloc(sizeof(struct name));

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

struct circleq *convexHull(SDL_Rect *Points, int n);

#endif /* __convexhull_h */