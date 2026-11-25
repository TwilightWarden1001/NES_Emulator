#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

int text_init();
void create_texture(SDL_Renderer*, const char*, SDL_Color);
void render_copy(SDL_Renderer *renderer);
void text_shutdown();

#endif