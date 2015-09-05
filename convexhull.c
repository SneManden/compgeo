#include "convexhull.h"

#define WINDOW_TITLE "Convex Hull Implementation"
#define POINT_SIZE 5
#define NUMBER_OF_POINTS 100
#define CIRCLE_RADIUS 275
#define MAX_HEIGHT 1000

SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Event event;
SDL_Renderer *renderer;

CIRCLEQ_HEAD(circleq, entry);
struct entry {
    SDL_Rect *rect;
    CIRCLEQ_ENTRY(entry) entries;
} *p, *q;

void drawPre(SDL_Renderer *renderer) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // white
    SDL_RenderClear(renderer);
}

void drawPost(SDL_Renderer *renderer) {
    // Update screen
    SDL_RenderPresent(renderer);
}

void drawPoints(SDL_Renderer *renderer, SDL_Rect *points, int n) {
    // Draw points
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // black
    SDL_RenderFillRects(renderer, points, n);
}

void drawConvexHull(SDL_Renderer *renderer, struct circleq *chull) {
    // Draw lines
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // red
    SDL_Rect *u, *v;
    for (p=chull->cqh_first; p->entries.cqe_next!=(void*)chull; p=p->entries.cqe_next) {
        u = p->rect;
        // Distinguish convex-hull-defining points
        SDL_RenderFillRect(renderer, u);
        v = p->entries.cqe_next->rect;
        SDL_RenderDrawLine(renderer,
            u->x+u->w/2, u->y+u->h/2,
            v->x+v->w/2, v->y+v->h/2);
    }
    // Draw line between head and tail
    u = chull->cqh_first->rect;
    v = chull->cqh_last->rect;
    SDL_RenderDrawLine(renderer,
        u->x+u->w/2, u->y+u->h/2,
        v->x+v->w/2, v->y+v->h/2);
}

void printUsage(void) {
    printf("Convex hull implementation:"
           "  computed on randomly distributed points\n"
           "  within a circle of given radius.\n");
    printf("Parameters:\n");
    printf("\t[1] Number of points (default: 100)\n");
    printf("\t[2] Points radius (default: 100)\n");
}

int main(int argc, char **argv) {
    int WINDOW_WIDTH = 800,
        WINDOW_HEIGHT = 600;

    printUsage();

    int number_of_points = NUMBER_OF_POINTS,
        circle_radius = CIRCLE_RADIUS;
    if (argc >= 2) number_of_points = atoi(argv[1]);
    if (argc >= 3) circle_radius = atoi(argv[2]);
    printf("Using values:\n"
           "\t[1] %5d (Number of points)\n"
           "\t[2] %5d (Points radius)\n", number_of_points, circle_radius);
    if (circle_radius > CIRCLE_RADIUS) {
        WINDOW_HEIGHT = min( 2*circle_radius + 50, MAX_HEIGHT);
        WINDOW_WIDTH = 4*WINDOW_HEIGHT / 3;
    }

    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"); 

    // Create window and surface (unused for now)
    window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create points
    // http://stackoverflow.com/questions/5837572/generate-a-random-point-within-a-circle-uniformly
    SDL_Rect *points = calloc(number_of_points, sizeof(SDL_Rect));
    double t, r;
    for (int i=0; i<number_of_points; i++) {
        r = circle_radius * sqrt( ((double)rand()/(double)RAND_MAX) );
        t = 2 * 3.14159265 * ((double)rand() / (double)RAND_MAX);
        points[i].x = WINDOW_WIDTH/2 + r * cos(t);
        points[i].y = WINDOW_HEIGHT/2 + r * sin(t);
        points[i].w = POINT_SIZE;
        points[i].h = POINT_SIZE;
    }

    // Convex hull
    struct circleq *chull = convexHull(points, number_of_points);

    // Draw everything once
    drawPre(renderer);
    drawPoints(renderer, points, number_of_points);
    drawConvexHull(renderer, chull);
    drawPost(renderer);

    // Main loop
    int quit = false;
    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    // Quit and free resources
    // TODO: free lists too
    free(points);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


// Compares elements with (x,y) coordinates, ordering lexicographicaly
// https://en.wikipedia.org/wiki/Graham_scan#Pseudocode
static int cmp(const void *a, const void *b) {
    const SDL_Rect *p = a, *q = b;
    if (p->x == q->x) return (p->y) > (q->y);
    else              return (p->x) > (q->x);
}

/**
 * Three points are a counter-clockwise turn if ccw > 0, clockwise if
 * ccw < 0, and collinear if ccw = 0 because ccw is a determinant that
 * gives twice the signed  area of the triangle formed by p1, p2 and p3.
 */
int ccw(struct circleq *head) {
    struct entry *p3 = head->cqh_last,
                 *p2 = p3->entries.cqe_prev,
                 *p1 = p2->entries.cqe_prev;
    return (p2->rect->x - p1->rect->x)*(p3->rect->y - p1->rect->y)
         - (p2->rect->y - p1->rect->y)*(p3->rect->x - p1->rect->x);
}
int lastThreeMakeRightTurn(struct circleq *head) {
    return ccw(head) > 0;
}

/**
 * Computes the convex hull [CH()].
 * Implementation based on algorithm from Chapter 1, page 6 [CompGeo08]
 * @param  Points A set of points (in the plane)
 * @param  n      Number of points in Points
 * @return        A list containing the vertices of CH(P) in clockwise order
 */
struct circleq *convexHull(SDL_Rect *Points, int n) {
    printf("convexHull(points, %d):\n", n);
    int listSize = 0;
    qsort(Points, n, sizeof(SDL_Rect), cmp);

    // Upper part
    CIRCLEQ_NEW(circleq, Lupper);
    CIRCLEQ_INIT(Lupper);
    p = malloc(sizeof(struct entry));
    q = malloc(sizeof(struct entry));
    p->rect = &Points[0];
    q->rect = &Points[1];
    CIRCLEQ_INSERT_HEAD(Lupper, p, entries);
    CIRCLEQ_INSERT_TAIL(Lupper, q, entries);
    listSize = 2;
    for (int i=2; i<n; i++) {
        p = malloc(sizeof(struct entry));
        p->rect = &Points[i];
        CIRCLEQ_INSERT_TAIL(Lupper, p, entries);
        listSize++;
        while (listSize>2 && !lastThreeMakeRightTurn(Lupper)) {
            q = Lupper->cqh_last->entries.cqe_prev; // next to last element
            CIRCLEQ_REMOVE(Lupper, q, entries);
            listSize--;
        }
    }
    // Lower part
    CIRCLEQ_NEW(circleq, Lower);
    CIRCLEQ_INIT(Lower);
    p = malloc(sizeof(struct entry));
    q = malloc(sizeof(struct entry));
    p->rect = &Points[n-1];
    q->rect = &Points[n-2];
    CIRCLEQ_INSERT_HEAD(Lower, p, entries);
    CIRCLEQ_INSERT_TAIL(Lower, q, entries);
    listSize = 2;
    for (int i=n-3; i>=0; i--) {
        p = malloc(sizeof(struct entry));
        p->rect = &Points[i];
        CIRCLEQ_INSERT_TAIL(Lower, p, entries);
        listSize++;
        while (listSize>2 && !lastThreeMakeRightTurn(Lower)) {
            q = Lower->cqh_last->entries.cqe_prev;
            CIRCLEQ_REMOVE(Lower, q, entries);
            listSize--;
        }
    }
    // NB: inserting expressions for p and q in CIRCLEQ_REMOVE leads to serious problems
    p = Lower->cqh_first;
    CIRCLEQ_REMOVE(Lower, p, entries);
    q = Lower->cqh_last;
    CIRCLEQ_REMOVE(Lower, q, entries);
    // Ugly list-merge (manual merges hangs the program? :/)
    CIRCLEQ_FOREACH_SAFE(p, Lower, entries, q) {
        CIRCLEQ_INSERT_TAIL(Lupper, p, entries);
    }

    printf("Done: Convex Hull computed.\n");
    return Lupper;
}


/**
 * [CompGeo08]
 *     Computational Geometry: Algorithms and Applications, Third edition,
 *     Mark de Berg, Otfried Cheong, Marc van Kreveld, Mark Overmars
 */