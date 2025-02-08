#include "renderer.h"

void renderer_init(renderer_data** data) {
#ifdef SNAKE_PLATFORM_SDL
    *data = (renderer_data*) malloc(sizeof(renderer_data));

    (*data)->window = SDL_CreateWindow(SNAKE_SDL_TITLE, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        GAME_W * GAME_UNIT_PX, GAME_H * GAME_UNIT_PX, 0);
    (*data)->renderer = SDL_CreateRenderer((*data)->window, -1, SDL_RENDERER_ACCELERATED);
    (*data)->event = (SDL_Event*) malloc(sizeof(SDL_Event));

#elif defined(SNAKE_PLATFORM_GC)
    *data = (renderer_data*) malloc(sizeof(renderer_data));
    GRRLIB_Init();

    PAD_Init();

    GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
    (*data)->screenWidth = rmode->fbWidth;
    (*data)->screenHeight = rmode->efbHeight;
#elif defined(SNAKE_PLATFORM_3DS)
    *data = (renderer_data*) malloc(sizeof(renderer_data));
    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, NULL);

    (*data)->top = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);

    printf("Welcome to Snake! Press Start to quit.\n");
#endif
}

snake_dir renderer_getinput(renderer_data** data) {
#ifdef SNAKE_PLATFORM_SDL
    while (SDL_PollEvent((*data)->event)) {
        switch ((*data)->event->type) {
            case SDL_QUIT:
                renderer_cleanup(data);
                break;
            case SDL_KEYDOWN:
                switch ((*data)->event->key.keysym.sym) {
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

#elif defined(SNAKE_PLATFORM_GC)
    PAD_ScanPads();

    u16 buttons = PAD_ButtonsDown(0);
    u16 is_start = PAD_ButtonsDown(0) & PAD_BUTTON_START;
    u16 is_left = PAD_ButtonsDown(0) & PAD_BUTTON_LEFT;
    u16 is_right = PAD_ButtonsDown(0) & PAD_BUTTON_RIGHT;
    u16 is_up = PAD_ButtonsDown(0) & PAD_BUTTON_UP;
    u16 is_down = PAD_ButtonsDown(0) & PAD_BUTTON_UP;

    s8 sx = PAD_StickX(0);
    s8 sy = PAD_StickY(0);

    if (is_start) {
        renderer_cleanup(data);
    } else if (is_left || sx < 0) {
        return LEFT;
    } else if (is_right || sx > 0) {
        return RIGHT;
    } else if (is_up || sy > 0) {
        return UP;
    } else if (is_down || sy < 0) {
        return DOWN;
    }
#elif defined(SNAKE_PLATFORM_3DS)
    if(!aptMainLoop()) {
        renderer_cleanup(data);
    }

    hidScanInput();

    u32 keys = hidKeysDown();

    u32 is_start = keys & KEY_START;
    u32 is_left = keys & KEY_LEFT;
    u32 is_right = keys & KEY_RIGHT;
    u32 is_up = keys & KEY_UP;
    u32 is_down = keys & KEY_DOWN;

    if (is_start) {
        renderer_cleanup(data);
    } else if (is_left) {
        return LEFT;
    } else if (is_right) {
        return RIGHT;
    } else if (is_up) {
        return UP;
    } else if (is_down) {
        return DOWN;
    }
#endif
    return NONE;
}

void renderer_loop(renderer_data** data, snake* s, s_point* apple) {
#ifdef SNAKE_PLATFORM_SDL
    SDL_SetRenderDrawColor((*data)->renderer, 255, 255, 255, 255);
    SDL_RenderClear((*data)->renderer);

    for(int i = 0; i < s->part_count; i++) {        
        SDL_Rect part_rect = (SDL_Rect) {
            s->parts[i].x * GAME_UNIT_PX, s->parts[i].y * GAME_UNIT_PX,
            GAME_UNIT_PX, GAME_UNIT_PX
        };

        SDL_SetRenderDrawColor((*data)->renderer, 0, 0, 0, 255);
        SDL_RenderFillRect((*data)->renderer, &part_rect);
    }

    SDL_Rect apple_rect = (SDL_Rect) {
        apple->x * GAME_UNIT_PX, apple->y * GAME_UNIT_PX,
        GAME_UNIT_PX, GAME_UNIT_PX
    };

    SDL_SetRenderDrawColor((*data)->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect((*data)->renderer, &apple_rect);

    SDL_RenderPresent((*data)->renderer);
    SDL_Delay(50);
#elif defined(SNAKE_PLATFORM_GC)
    GRRLIB_FillScreen(0xffffffff);

    for(int i = 0; i < s->part_count; i++) {        
        GRRLIB_Rectangle(
            s->parts[i].x * GAME_UNIT_PX,
            s->parts[i].y * GAME_UNIT_PX,
            GAME_UNIT_PX, GAME_UNIT_PX,
            0x000000ff, true
        );
    }

    GRRLIB_Rectangle(
        apple->x * GAME_UNIT_PX,
        apple->y * GAME_UNIT_PX,
        GAME_UNIT_PX, GAME_UNIT_PX,
        0xff0000ff, true
    );

    GRRLIB_Render();
#elif defined(SNAKE_PLATFORM_3DS)
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear((*data)->top, C2D_Color32(0xff, 0xff, 0xff, 0xff));
    C2D_SceneBegin((*data)->top);

    for(int i = 0; i < s->part_count; i++) {        
        C2D_DrawRectSolid(
            s->parts[i].x * GAME_UNIT_PX,
            s->parts[i].y * GAME_UNIT_PX,
            0, GAME_UNIT_PX, GAME_UNIT_PX,
            C2D_Color32(0x00, 0x00, 0x00, 0xff)
        );
    }
    
    C2D_DrawRectSolid(
        apple->x * GAME_UNIT_PX,
        apple->y * GAME_UNIT_PX,
        0, GAME_UNIT_PX, GAME_UNIT_PX,
        C2D_Color32(0xff, 0x00, 0x00, 0xff)
    );

    C3D_FrameEnd(0);
    svcSleepThread(50000000);
#endif
}

void renderer_cleanup(renderer_data** data) {
#ifdef SNAKE_PLATFORM_SDL
    SDL_DestroyRenderer((*data)->renderer);
    SDL_DestroyWindow((*data)->window);
    SDL_Quit();
    exit(0);
#elif defined(SNAKE_PLATFORM_GC)
    GRRLIB_Exit();
    exit(0);
#elif defined(SNAKE_PLATFORM_3DS)
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    exit(0);
#endif
}