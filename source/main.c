#include "renderer.h"
#include <time.h>

int main() {
    srand(time(0));

    renderer_data* data;
    renderer_init(&data);

    snake s;
    snake_new(&s);

    s_point apple = (s_point) {GAME_W / 2, GAME_H / 2};

    // Some platforms (e.g. Gamecube)
    // have trouble with sleeping on the rendering thread
    // and will make the output flickery.

    // These will need to define NO_SLEEP in platform.h

    // My workaround is to handle snake logic every third frame
    // 60 Hz (~60 FPS without logic time) / 3 = 20 FPS (target FPS)
    #ifdef NO_SLEEP
    unsigned char frame_count = 0;
    #endif

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

        #ifdef NO_SLEEP
        // Handle every third frame
        if(frame_count % 3 == 0) {
        #endif
        snake_tick(&s, &apple);
        #ifdef NO_SLEEP
        }

        frame_count++;
        // Loop the counter around 10
        frame_count = frame_count % 10;
        #endif

        renderer_loop(&data, &s, &apple);
    }

    renderer_cleanup(&data);
}