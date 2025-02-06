#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// The size of a game unit in pixels
#define GAME_UNIT_PX = 15

// Size of the game world in game units
#define GAME_W 50
#define GAME_H 50

// Maximum size of a Snake
#define MAX_SNAKE 10

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

snake* snake_new();
void snake_tick(snake* s, s_point* apple);
void snake_add_part(snake* s);