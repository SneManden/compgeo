#include <SDL2/SDL.h>

#define WINDOW_TITLE "Hello world"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 640

SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Event event;

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    screen = SDL_GetWindowSurface(window);

    int loop = 1;
    while (loop) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                loop = 0;
                break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}