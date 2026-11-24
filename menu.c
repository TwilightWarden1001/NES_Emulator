#include "menu.h"
#include "text.h"

void menu_init(SDL_Renderer *renderer) {
    // Eventually the name and color will be passed in as well
    SDL_Color color = { 30, 30, 30, 255 };
    create_texture(renderer, "File", color);
}

void draw_menu(SDL_Renderer *renderer) {
    SDL_Rect rectangle = { 0, 0, 256, HEIGHT }; // X, Y, Width, Height;
    SDL_SetRenderDrawColor(renderer, 245, 245, 220, 255);
    SDL_RenderFillRect (renderer, &rectangle);
    SDL_RenderDrawRect (renderer, &rectangle);
    render_copy(renderer, &rectangle);
    return;
}