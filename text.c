#include "text.h"

static TTF_Font *font = NULL;
static SDL_Texture *file_texture = NULL;
static int texture_width = 1;
static int texture_height = 1;

int text_init() {
    // TTF_Init returns 0 on success.
    if (TTF_Init() < 0) {
        printf("TTF_Init failed. %s\n", TTF_GetError());
        return -1;
    }

    // TTF_OpenFont returns NULL when it's not successful.
    font = TTF_OpenFont("menu.ttf", 16);
    if (font == NULL) {
        printf("TTF_OpenFont failed. %s\n", TTF_GetError());
        return -1;
    }
    return 0;
}

void create_texture(SDL_Renderer *renderer, const char *text, SDL_Color color) {
    // First we create a surface
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL) {
        printf("TTF_RenderText_Blended failed. %s\n", TTF_GetError());
        return;
    }

    // Then from that surface we create a texture
    file_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (file_texture == NULL) {
        printf("SDL_CreateTextureFromSurface failed. %s\n", TTF_GetError());
        return;
    }

    SDL_QueryTexture(file_texture, NULL, NULL, &texture_width, &texture_height);

    // Needs Error Handling
    SDL_FreeSurface(surface);
    return;
}

void render_copy(SDL_Renderer *renderer, SDL_Rect *rectangle) {
    SDL_RenderCopy(renderer, file_texture, NULL, rectangle);
    return;
}

void text_shutdown() {
    SDL_DestroyTexture(file_texture);
    TTF_CloseFont(font);
    TTF_Quit();
    return;
}