#include "snake.h"

snake* snake_new() {
    snake* s = (snake*) malloc(sizeof(snake));
    
    s->parts[0] = (s_point) {GAME_W / 2, GAME_H / 2};
    s->part_count = 0;
    
    s->direction = RIGHT;
    s->is_alive = true;

    return s;
}

void snake_tick(snake* s, s_point* apple) {
    // Don't process if the snake is dead
    if (!s->is_alive) {
        return;
    }
    
    // Get a reference to the snake's head
    s_point* head = &(s->parts[s->part_count]);

    // Loop the position around the game world
    head->x = head->x % GAME_W;
    head->y = head->y % GAME_H;

    // Loop negative positions
    if (head->x < 0) {
        head->x = GAME_W;
    }

    if (head->y < 0) {
        head->y = GAME_H;
    }

    // Check potential collisions with the apple
    if (head->x == apple->x && head->y == apple->y) {
        snake_add_part(s);

        printf("aple\n");
    } else {
        for (int i = 0; i < s->part_count-1; i++) {
            s_point* part = &(s->parts[i]);
            s_point* part_n = &(s->parts[i + 1]);

            part->x = part_n->x;
            part->y = part_n->y;
        }
    }
}

void snake_add_part(snake* s) {
    if(s->part_count >= MAX_SNAKE-1) {
        return;
    }

    s->parts[s->part_count+1].x = s->parts[s->part_count].x;
    s->parts[s->part_count+1].y = s->parts[s->part_count].y;
    s->part_count++;
}

