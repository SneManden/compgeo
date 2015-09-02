#include <math.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "convexhull.h"

/**
 * [CompGeo08]
 *     Computational Geometry: Algorithms and Applications, Third edition,
 *     Mark de Berg, Otfried Cheong, Marc van Kreveld, Mark Overmars
 */

#define WINDOW_TITLE "Hello world"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 640

#define POINTS 1000
#define POINT_SIZE 3

SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Event event;
SDL_Renderer *renderer;

void draw(SDL_Renderer *renderer, SDL_Rect *points) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // white
    SDL_RenderClear(renderer);

    // Set color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // black
    // Draw points
    SDL_RenderFillRects(renderer, points, POINTS);

    // Update screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"); 

    window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create points
    // http://stackoverflow.com/questions/5837572/generate-a-random-point-within-a-circle-uniformly
    SDL_Rect *points = calloc(POINTS, sizeof(SDL_Rect));
    int radius = 200;
    double t, r;
    for (int i=0; i<POINTS; i++) {
        r = radius * sqrt( ((double)rand()/(double)RAND_MAX) );
        t = 2 * 3.14159265 * ((double)rand() / (double)RAND_MAX);
        points[i].x = WINDOW_WIDTH/2 + r * cos(t);
        points[i].y = WINDOW_HEIGHT/2 + r * sin(t);
        points[i].w = POINT_SIZE;
        points[i].h = POINT_SIZE;
        printf("point %3d {x:%3d, y:%3d}\n", i, points[i].x, points[i].y);
    }

    // main loop
    int quit = false;
    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
        }

        draw(renderer, points);
    }
    free(points);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


/**
 * Computes the convex hull [CH()]. Algorithm from Chapter 1, page 6 [CompGeo08]
 * This implementation is modified to deal with issues:
 *  - N/A
 * @param  P A set P of points (in the plane)
 * @return   A list L containing the vertices of CH(P) in clockwise order
 */
/*List *convexHull(Set *P) {
    int n = setSize(P);
    List *Lup = listNew();
    // 1. Sort points by x and y coordinate
    List *sP = setSorted(P); // TOOD: sort function pointer [by (x,y)]

    // Upper part
    ListNode p, q, pi;
    p = listHead(sP);
    q = p->next;
    pi = q;
    listAdd(Lup, p);
    listAdd(Lup, q);    
    for (int i=2; i<n; i++) {
        pi = pi->next;
        listAdd(Lup, pi);
        // ... and last three points in Lup do not make a right turn
        while (listSize(Lup)>2 && !lastThreeMakeRightTurn(Lup)) {
            // Delete the middle of the last three points from Lup
            listRemove(Lup, listTail(Lup)->prev);
        }
    }
    // Lower part
    p = listTail(Lup);
    q = p->prev;
    pi = q;
    List *Low = listNew();
    listAdd(Low, p);
    listAdd(Low, q);
    for (int i=n-3; i>=0; i--) {
        pi = pi->prev;
        listAdd(Low, pi);
        // ... and last three points in Low do not make a right turn
        while (listSize(Low)>2 && !lastThreeMakeRightTurn(Low)) {
            // Delete the middle of the last three points from Low
            listRemove(Low, listTail(Low)->prev);
        }
    }
    // Remove first and last points from Low to avoid duplication
    listRemove(Low, listHead(Low));
    listRemove(Low, listTail(Low));

    List *L = listMerge(Lup, Low); // Append Low to Lup
    return L;
}*/