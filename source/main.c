#include "renderer.h"
#include <time.h>

int main() {
    srand(time(0));

    renderer_data* data;
    renderer_init(&data);

    snake s;
    snake_new(&s);

    s_point apple = (s_point) {25, 25};

    while(true) {
        snake_dir dir = renderer_getinput(&data);
        
        if (dir != NONE && (
            ((dir == LEFT) && (s.direction != RIGHT)) ||
            ((dir == RIGHT) && (s.direction != LEFT)) ||
            ((dir == UP) && (s.direction != DOWN)) ||
            ((dir == DOWN) && (s.direction != UP))
        )) {
            s.direction = dir;
        }

        snake_tick(&s, &apple);
        renderer_loop(&data, &s, &apple);
    }

    renderer_cleanup(&data);
}