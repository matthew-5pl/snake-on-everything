#include <stdio.h>

#include "platform.h"
#include "snake.h"

// Every platform can store different renderer data as needed
typedef struct {
#if defined(SNAKE_PLATFORM_SWITCH)
    PadState pad;
#endif
#if defined(HAS_SDL)
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Event* event;
#elif defined(SNAKE_PLATFORM_GC) || defined(SNAKE_PLATFORM_WII)
    int screenHeight;
    int screenWidth;
#elif defined(SNAKE_PLATFORM_3DS)
    C3D_RenderTarget* top;
#endif
#if defined(SNAKE_PLATFORM_WIIU)
    VPADStatus gamepad;
    VPADReadError error;
#endif
} renderer_data;

void renderer_init(renderer_data** data);

snake_dir renderer_getinput(renderer_data** data);
void renderer_loop(renderer_data** data, snake* s, s_point* apple);

void renderer_cleanup(renderer_data** data);
