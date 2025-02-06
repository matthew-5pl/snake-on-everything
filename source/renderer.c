#include "renderer.h"

void renderer_init(renderer_data* data) {
#ifdef SNAKE_PLATFORM_SDL
    data = (renderer_data*) malloc(sizeof(renderer_data));

    data->window = SDL_CreateWindow(SNAKE_SDL_TITLE, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, 0);
    data->renderer = SDL_CreateRenderer(data->window, -1, SDL_RENDERER_ACCELERATED);
#endif
}

snake_dir renderer_getinput(renderer_data* data) {
#ifdef SNAKE_PLATFORM_SDL
    while (SDL_PollEvent(&(data->event))) {
        switch (data->event.type) {
            case SDL_QUIT:
                renderer_cleanup(data);
                break;
            case SDL_KEYDOWN:
                switch (data->event.key.keysym.sym) {
                    case SDLK_w:
                        return UP;
                        break;
                    case SDLK_a:
                        return LEFT;
                        break;
                    case SDLK_s:
                        return DOWN;
                        break;
                    case SDLK_d:
                        return RIGHT;
                        break;
                }
                break;
        }
    }
#endif

    return NONE;
}

void renderer_loop(renderer_data* data, snake* s, s_point* apple) {
#ifdef SNAKE_PLATFORM_SDL
    
#endif
}