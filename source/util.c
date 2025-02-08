#include "util.h"

void sleep_ms(int milliseconds) {
    struct timespec req;
    req.tv_sec = milliseconds / 1000;  // Seconds
    req.tv_nsec = (milliseconds % 1000) * 1000000; // Nanoseconds

    nanosleep(&req, NULL); // Pause execution
}