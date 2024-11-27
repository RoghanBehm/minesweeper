#include <iostream>
#include <SDL2/SDL.h>
#include "render.hpp"
#include "settings.hpp"

void draw_cell(SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect rect = {x, y, globalSettings.cell_size, globalSettings.cell_size};

    // Fill colour
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    // Top side (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect top = {rect.x, rect.y, rect.w, thickness};
    SDL_RenderFillRect(renderer, &top);

    // Bottom side (grey)
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
    SDL_Rect bottom = {rect.x, rect.y + rect.h - thickness, rect.w, thickness};
    SDL_RenderFillRect(renderer, &bottom);

    // Left side (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect left = {rect.x, rect.y, thickness, rect.h};
    SDL_RenderFillRect(renderer, &left);

    // Right side (grey)
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
    SDL_Rect right = {rect.x + rect.w - thickness, rect.y, thickness, rect.h};
    SDL_RenderFillRect(renderer, &right);
}
