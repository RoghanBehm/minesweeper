#include <iostream>
#include <SDL2/SDL.h>
#include "render.hpp"
#include "settings.hpp"
#include "game.hpp"

void draw_cell(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released, Node& cell)
{

    if ((clicked & released) | cell.isRevealed) {
        SDL_Rect rect = {x, y, globalSettings.cell_size, globalSettings.cell_size};
        
        // Fill colour
        SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
        SDL_RenderFillRect(renderer, &rect);
        cell.isRevealed = true;
        released = false;
        return;
    }
     else if (clicked)
    {
        SDL_Rect rect = {x, y, globalSettings.cell_size, globalSettings.cell_size};
        
        // Fill colour
        SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
        SDL_RenderFillRect(renderer, &rect);
        return;
    } 
    SDL_Rect rect = {x, y, globalSettings.cell_size, globalSettings.cell_size};

    // Fill colour
    SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Top side (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect top = {rect.x + inset, rect.y, rect.w - 2 * inset, thickness};
    SDL_RenderFillRect(renderer, &top);

    // Bottom side (grey)
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
    SDL_Rect bottom = {rect.x + inset, rect.y + rect.h - thickness, rect.w - 2 * inset, thickness};
    SDL_RenderFillRect(renderer, &bottom);

    // Left side (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect left = {rect.x, rect.y + inset, thickness, rect.h - 2 * inset};
    SDL_RenderFillRect(renderer, &left);

    // Right side (grey)
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
    SDL_Rect right = {rect.x + rect.w - thickness, rect.y + inset, thickness, rect.h - 2 * inset};
    SDL_RenderFillRect(renderer, &right);
}
