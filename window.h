#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <stdbool.h>

// First two are width and length. Third is the default scale (multiply both by the scale)
#define NES_SCREEN_WIDTH 256
#define NES_SCREEN_HEIGHT 240
#define WINDOW_SCALE 3

typedef struct Window {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} Window;

bool window_init(Window *win);
void window_handle_events(Window *win);
void window_render(Window *win);
void window_destroy(Window *win);

#endif