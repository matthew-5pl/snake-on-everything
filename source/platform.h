#ifndef SNAKE_PLATFORM_H_

// SDL platforms
#if defined(_WIN32) || defined(WIN32) || defined(__unix__) || defined(__APPLE__)
#ifndef PLATFORM_VSCODE // Avoid making VSCode confused when working on other platforms
#define SNAKE_PLATFORM_SDL
#define SNAKE_SDL_TITLE "Snake"
#include <SDL2/SDL.h>
#endif
#endif

// Gamecube
#if defined(GAMECUBE)
#define SNAKE_PLATFORM_GC
#include <grrlib.h>
#include <ogc/video.h>
#include <ogc/pad.h>

#define GC_STICK_THRESHOLD 48
#define NO_SLEEP

// 3DS
#elif defined(__3DS__)
#define SNAKE_PLATFORM_3DS
#include <3ds.h>
#include <citro2d.h>

// DS
#elif defined(__NDS__)
#define SNAKE_PLATFORM_DS
#include <nds.h>
#include <gl2d.h>

#define printf(x) iprintf(x)

// Switch
#elif defined(__SWITCH__)
#define SNAKE_PLATFORM_SWITCH
#define SNAKE_SDL_TITLE "Snake"
#undef __APPLE__ 
// Or SDL2 will complain about some Apple specific header file when compiling on macOS
#include <SDL2/SDL.h>
// We can just use SDL2 for rendering here
#include <switch.h>
#endif

#define SNAKE_PLATFORM_H_
#endif