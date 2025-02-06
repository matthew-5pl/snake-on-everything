#include <stdio.h>

#if defined(_WIN32) || defined(WIN32) || defined(__unix__) || defined(__APPLE__)
#define SNAKE_PLATFORM_SDL
#define SNAKE_SDL_TITLE "Snake"
#include <SDL2/SDL.h>
#endif

#include "snake.h"

typedef struct {
#ifdef SNAKE_PLATFORM_SDL
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Event event;
#endif
} renderer_data;

void renderer_init(renderer_data* data);

snake_dir renderer_getinput(renderer_data* data);
void renderer_loop(renderer_data* data, snake* s, s_point* apple);

void renderer_cleanup(renderer_data* data);
