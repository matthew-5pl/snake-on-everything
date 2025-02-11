#include "renderer.h"

void renderer_init(renderer_data** data) {
    // Allocate memory for the renderer data
    *data = (renderer_data*) malloc(sizeof(renderer_data));
#ifdef SNAKE_PLATFORM_SDL // SDL platforms (Win, macOS, Linux...)
    (*data)->window = SDL_CreateWindow(SNAKE_SDL_TITLE, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        GAME_W * GAME_UNIT_PX, GAME_H * GAME_UNIT_PX, 0);
    (*data)->renderer = SDL_CreateRenderer((*data)->window, -1, SDL_RENDERER_ACCELERATED);
    (*data)->event = (SDL_Event*) malloc(sizeof(SDL_Event));
#elif defined(SNAKE_PLATFORM_SWITCH) // Switch
    (*data)->window = SDL_CreateWindow(SNAKE_SDL_TITLE, 
        0, 0,
        1920, 1080, 0);
    (*data)->renderer = SDL_CreateRenderer((*data)->window, -1, SDL_RENDERER_ACCELERATED);
    (*data)->event = (SDL_Event*) malloc(sizeof(SDL_Event));

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&((*data)->pad));
#elif defined(SNAKE_PLATFORM_GC) || defined(SNAKE_PLATFORM_WII) // Gamecube/Wii
    GRRLIB_Init();

    PAD_Init();

    #ifdef SNAKE_PLATFORM_WII
        WPAD_Init();
    #endif

    GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
    (*data)->screenWidth = rmode->fbWidth;
    (*data)->screenHeight = rmode->efbHeight;
#elif defined(SNAKE_PLATFORM_3DS) // 3DS
    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    consoleInit(GFX_BOTTOM, NULL);

    (*data)->top = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
#elif defined(SNAKE_PLATFORM_DS) // DS
    videoSetMode(MODE_5_3D);
    consoleDemoInit();

    glScreen2D();
#elif defined(SNAKE_PLATFORM_GBA) // GBA
    REG_DISPCNT= DCNT_MODE3 | DCNT_BG2;
#elif defined(SNAKE_PLATFORM_WIIU) // Wii U
    WHBProcInit();
    VPADInit();

    (*data)->window = SDL_CreateWindow(SNAKE_SDL_TITLE, 
        0, 0,
        1920, 1080, 0);
    (*data)->renderer = SDL_CreateRenderer((*data)->window, -1, SDL_RENDERER_ACCELERATED);
    (*data)->event = (SDL_Event*) malloc(sizeof(SDL_Event));
#endif
    // Not all platforms get to see this :(
    printf("Welcome to Snake! Press Start to quit.\n");
}

snake_dir renderer_getinput(renderer_data** data) {
#ifdef SNAKE_PLATFORM_SDL // SDL platforms (Win, macOS, Linux...)
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
#elif defined(SNAKE_PLATFORM_SWITCH) // Handle Switch input
    padUpdate(&((*data)->pad));

    u64 buttons = padGetButtonsDown(&((*data)->pad));

    u64 is_plus = buttons & HidNpadButton_Plus;
    u64 is_left = buttons & HidNpadButton_AnyLeft;
    u64 is_right = buttons & HidNpadButton_AnyRight;
    u64 is_up = buttons & HidNpadButton_AnyUp;
    u64 is_down = buttons & HidNpadButton_AnyDown;

    if (is_plus) {
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
#elif defined(SNAKE_PLATFORM_GC) || defined(SNAKE_PLATFORM_WII) // Handle Gamecube/Wii input
    PAD_ScanPads();

    u16 buttons = PAD_ButtonsDown(0);
    u16 is_start = buttons & PAD_BUTTON_START;
    u16 is_left = buttons & PAD_BUTTON_LEFT;
    u16 is_right = buttons & PAD_BUTTON_RIGHT;
    u16 is_up = buttons & PAD_BUTTON_UP;
    u16 is_down = buttons & PAD_BUTTON_DOWN;

    #ifdef SNAKE_PLATFORM_WII
    WPAD_ScanPads();
    // Handle Wii Remote Input
    u32 wii_buttons = WPAD_ButtonsDown(0);
    
    is_start |= (wii_buttons & WPAD_BUTTON_PLUS);
    is_left |= (wii_buttons & WPAD_BUTTON_LEFT);
    is_right |= (wii_buttons & WPAD_BUTTON_RIGHT);
    is_up |= (wii_buttons & WPAD_BUTTON_UP);
    is_down |= (wii_buttons & WPAD_BUTTON_DOWN);
    #endif

    s8 sx = PAD_StickX(0);
    s8 sy = PAD_StickY(0);

    if (is_start) {
        renderer_cleanup(data);
    } else if (is_left || sx < -GC_STICK_THRESHOLD) {
        return LEFT;
    } else if (is_right || sx > GC_STICK_THRESHOLD) {
        return RIGHT;
    } else if (is_up || sy > GC_STICK_THRESHOLD) {
        return UP;
    } else if (is_down || sy < -GC_STICK_THRESHOLD) {
        return DOWN;
    }
#elif defined(SNAKE_PLATFORM_3DS) // Handle 3DS input
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
#elif defined(SNAKE_PLATFORM_DS) // Handle DS input
    if(!pmMainLoop()) {
        renderer_cleanup(data);
    }

    scanKeys();

    u32 keys = keysDown();
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
#elif defined(SNAKE_PLATFORM_GBA) // Handle GBA input
    key_poll();

    u32 is_start = key_hit(KEY_START);
    u32 is_left = key_hit(KEY_LEFT);
    u32 is_right = key_hit(KEY_RIGHT);
    u32 is_up = key_hit(KEY_UP);
    u32 is_down = key_hit(KEY_DOWN);

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
#elif defined(SNAKE_PLATFORM_WIIU) // Handle Wii U input
    u32 result = VPADRead(VPAD_CHAN_0, &((*data)->gamepad), 1, &((*data)->error));

    if(result > 0 && (*data)->error == VPAD_READ_SUCCESS) {
        u32 buttons = (*data)->gamepad.hold;

        u32 is_plus = buttons & VPAD_BUTTON_PLUS;
        u32 is_left = buttons & VPAD_BUTTON_LEFT;
        u32 is_right = buttons & VPAD_BUTTON_RIGHT;
        u32 is_up = buttons & VPAD_BUTTON_UP;
        u32 is_down = buttons & VPAD_BUTTON_DOWN;

        VPADVec2D left_stick = (*data)->gamepad.leftStick;

        if (is_plus) {
            renderer_cleanup(data);
        } else if (is_left || left_stick.x < -WIIU_STICK_THRESHOLD) {
            return LEFT;
        } else if (is_right || left_stick.x > WIIU_STICK_THRESHOLD) {
            return RIGHT;
        } else if (is_up || left_stick.y > WIIU_STICK_THRESHOLD) {
            return UP;
        } else if (is_down || left_stick.y < -WIIU_STICK_THRESHOLD) {
            return DOWN;
        }
    }
#endif
    return NONE;
}

void renderer_loop(renderer_data** data, snake* s, s_point* apple) {
    // I also use SDL2 for rendering on the Switch
    // So the rendering code is shared with desktop platforms
#if defined(HAS_SDL)
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
#elif defined(SNAKE_PLATFORM_GC) || defined(SNAKE_PLATFORM_WII) // Handle Gamecube/Wii rendering (GRRLIB)
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
#elif defined(SNAKE_PLATFORM_3DS) // Handle 3DS rendering (citro2d)
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
#elif defined(SNAKE_PLATFORM_DS) // Handle DS rendering (gl2d)
    glBegin2D();
    glBoxFilled(
        0, 0,
        255, 191,
        RGB15(31, 31, 31)
    );

    for(int i = 0; i < s->part_count; i++) {        
        glBoxFilled(
            s->parts[i].x * GAME_UNIT_PX,
            s->parts[i].y * GAME_UNIT_PX,
            s->parts[i].x * GAME_UNIT_PX + GAME_UNIT_PX,
            s->parts[i].y * GAME_UNIT_PX + GAME_UNIT_PX,
            RGB15(0, 0, 0)
        );
    }

    glBoxFilled(
        apple->x * GAME_UNIT_PX,
        apple->y * GAME_UNIT_PX,
        apple->x * GAME_UNIT_PX + GAME_UNIT_PX,
        apple->y * GAME_UNIT_PX + GAME_UNIT_PX,
        RGB15(31, 0, 0)
    );

    glEnd2D();
    glFlush(0);
    swiWaitForVBlank();

    sleep_ms(50);
#elif defined(SNAKE_PLATFORM_GBA) // Handle GBA rendering (libtonc)
    vid_vsync();
    m3_fill(RGB15(31, 31, 31));

    for(int i = 0; i < s->part_count; i++) {              
        m3_rect(
            s->parts[i].x * GAME_UNIT_PX,
            s->parts[i].y * GAME_UNIT_PX,
            s->parts[i].x * GAME_UNIT_PX + GAME_UNIT_PX,
            s->parts[i].y * GAME_UNIT_PX + GAME_UNIT_PX,
            CLR_BLACK
        );
    }

    m3_rect(
        apple->x * GAME_UNIT_PX,
        apple->y * GAME_UNIT_PX,
        apple->x * GAME_UNIT_PX + GAME_UNIT_PX,
        apple->y * GAME_UNIT_PX + GAME_UNIT_PX,
        CLR_RED
    );
#endif
}

void renderer_cleanup(renderer_data** data) {
#if defined(HAS_SDL)
    SDL_DestroyRenderer((*data)->renderer);
    SDL_DestroyWindow((*data)->window);
    SDL_Quit();
#elif defined(SNAKE_PLATFORM_GC) || defined(SNAKE_PLATFORM_WII)
    GRRLIB_Exit();
#elif defined(SNAKE_PLATFORM_3DS)
    C2D_Fini();
    C3D_Fini();
    gfxExit();
#endif
#if defined(SNAKE_PLATFORM_WIIU)
    VPADShutdown();
    WHBProcStopRunning();
    WHBProcShutdown();
#endif
    exit(0);
}