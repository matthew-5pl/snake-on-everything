#include "snake.h"

void snake_new(snake* s) {    
    s->parts[0] = (s_point) {GAME_W / 2, GAME_H / 2};
    s->parts[1] = (s_point) {(GAME_W / 2) + 1, GAME_H / 2};
    s->parts[2] = (s_point) {(GAME_W / 2) + 2, GAME_H / 2};

    s->part_count = 3;
    
    s->direction = RIGHT;
    s->is_alive = true;
}

void snake_tick(snake* s, s_point* apple) {
    // Don't process if the snake is dead
    if (!s->is_alive) {
        return;
    }
    
    // Get a reference to the snake's head
    s_point* head = &(s->parts[s->part_count-1]);

    switch(s->direction) {
        case RIGHT:
            head->x++;
            break;
        case LEFT:
            head->x--;
            break;
        case UP:
            head->y--;
            break;
        case DOWN:
            head->y++;
            break;
    }

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

        apple->x = rand() % randrange(0, GAME_W - 1);
        apple->y = rand() % randrange(0, GAME_H - 1);

        printf("You got an apple!\n");
    } else {
        // Check for collisions with other parts
        for (int i = 0; i < s->part_count - 1; i++) {
            for (int j = 0; j < s->part_count - 1; j++) {
                if (i == j) {
                    continue;
                }

                if (s->parts[i].x == s->parts[j].x &&
                    s->parts[i].y == s->parts[j].y &&
                    i != j+1) {
                    s->is_alive = false;

                    printf("You're dead :(\n");

                    // Randomize the position of the apple
                    apple->x = rand() % GAME_W;
                    apple->y = rand() % GAME_H;

                    // Reset the snake
                    snake_new(s);
                }
            }

            s->parts[i].x = s->parts[i + 1].x;
            s->parts[i].y = s->parts[i + 1].y;
        }
    }
}

void snake_add_part(snake* s) {
    if(s->part_count >= MAX_SNAKE-1) {
        return;
    }

    s->parts[s->part_count].x = s->parts[s->part_count-1].x;
    s->parts[s->part_count].y = s->parts[s->part_count-1].y;
    s->part_count++;
}

