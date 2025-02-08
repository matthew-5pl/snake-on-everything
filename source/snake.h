#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "platform.h"
#include "util.h"

// The size of a game unit in pixels
// Size of the game world in game units

// Most platforms define their own,
// since every screen is different
#if defined(SNAKE_PLATFORM_SDL)
#define GAME_W 50
#define GAME_H 50
#define GAME_UNIT_PX 15
#elif defined(SNAKE_PLATFORM_GC)
#define GAME_W 42
#define GAME_H 35
#define GAME_UNIT_PX 15
#elif defined(SNAKE_PLATFORM_3DS)
#define GAME_W 50
#define GAME_H 30
#define GAME_UNIT_PX 8
#elif defined(SNAKE_PLATFORM_DS)
#define GAME_W 42
#define GAME_H 32
#define GAME_UNIT_PX 6
#elif defined(SNAKE_PLATFORM_SWITCH)
#define GAME_W 64
#define GAME_H 36
#define GAME_UNIT_PX 30
#endif

// Maximum size of a Snake
// Who wants to bother using realloc()? Not me!
#define MAX_SNAKE 50

// The snake's direction
typedef enum snake_dir {
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
} snake_dir;

typedef struct {
    int x, y;
} s_point;

typedef struct {
    s_point parts[MAX_SNAKE];
    int part_count;

    bool is_alive;
    snake_dir direction;
} snake;

void snake_new(snake* s);
void snake_tick(snake* s, s_point* apple);
void snake_add_part(snake* s);