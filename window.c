#include "window.h"
#include <stdio.h>

bool window_init(Window *win) {
    // First we try to initialize the video thing.
    // SDL_INIT returns 0 on success.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_INIT_VIDEO failed. %s\n", SDL_GetError());
        return false;
    }

    // Eventually we'll have to add stuff like audio and joystick/game controller.
    // Second, make the window, variables are explained
    win->window = SDL_CreateWindow(
        "NES Emulator",                   // Title
        SDL_WINDOWPOS_CENTERED,           // X Position
        SDL_WINDOWPOS_CENTERED,           // Y Position
        NES_SCREEN_WIDTH * WINDOW_SCALE,  // Width 256 * Whatever scale. Default is 3
        NES_SCREEN_HEIGHT * WINDOW_SCALE, // Height 240 * Whatever scale. Default is 3
        0                                 // Any extra flags. Note that SDL_WINDOW_SHOWN is not needed unless SDL_WINDOW_HIDDEN is set.
    );
    if (win->window == NULL) {
        printf("SDL_CreateWindow failed. %s\n", SDL_GetError());
        SDL_Quit();   // ONLY deals with SDL shit, NOT with our function.
        return false;
    }

    // Third make the renderer. This is what the PPU will actually use
    win->renderer = SDL_CreateRenderer(
        win->window, // The window we just created
        -1,          // Terrible magic number. Just initializes the first rendering driver
        SDL_RENDERER_ACCELERATED // Really useful suggestion by Muse. It makes it so that the GPU handles rendering, not the CPU.
        // SERIOUS FLAG. If there is not a GPU, this will fail. How you'd get a computer in 2026 with NO GPU is beyond me but still. 
    );
    if (win->renderer == NULL) {
        printf("SDL_CreateRenderer failed. %s\n", SDL_GetError());
        SDL_DestroyWindow(win->window); // Required so we don't 
        SDL_Quit();
        return false;
    }

    // Third, this just means that we treat everything as 256 by 240. The renderer scales it up based on other things
    int logical_render = SDL_RenderSetLogicalSize(
        win->renderer,
        NES_SCREEN_WIDTH,
        NES_SCREEN_HEIGHT
    );
    // This CAN Fail and if it does, it needs to be cleaned up
    if (logical_render < 0) {
        SDL_DestroyRenderer(win->renderer);
        SDL_DestroyWindow(win->window);
        SDL_Quit();
        return false;
    }

    // FINALLY if all the checks work we set 
    win->running = true;
    return true;
}

void window_handle_events(Window *win) {
    SDL_Event event;

    // Not much to say, just closes it when needed
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                win->running = false;
                break;  
            case SDL_KEYDOWN:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    win->running = false;
                    break;
                }
            default:
                break;
        }
    }
}

void window_render(Window *win) {
    SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 255); // Drawing operations. Obviously will be done by the PPU. 
    SDL_RenderClear(win->renderer);   // Again we do this for like, it's because it's a queue or something idk
    SDL_RenderPresent(win->renderer); // Something about a backbuffer idk
}

void window_destroy(Window *win) {
    if (win->renderer != NULL) {
        SDL_DestroyRenderer(win->renderer);
    }
    if (win->window   != NULL) {
        SDL_DestroyWindow(win->window);
    }
    SDL_Quit();
}