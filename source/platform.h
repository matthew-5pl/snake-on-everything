#ifndef SNAKE_PLATFORM_H_
// SDL platforms
#if defined(_WIN32) || defined(WIN32) || defined(__unix__) || defined(__APPLE__)

#define SNAKE_PLATFORM_SDL
#define SNAKE_SDL_TITLE "Snake"
#include <SDL2/SDL.h>

// Gamecube
#elif defined(GAMECUBE)

#define SNAKE_PLATFORM_GC
#include <grrlib.h>
#include <ogc/video.h>
#include <ogc/pad.h>
#endif

#define SNAKE_PLATFORM_H_
#endif