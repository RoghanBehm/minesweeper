#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "render.hpp"
#include "settings.hpp"
#include "game.hpp"

void draw_cell(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released, Node& cell, GameAssets &assets, int nearbyMines, bool &rightClick)
{
    SDL_Rect rect = {x, y, globalSettings.cell_size, globalSettings.cell_size};

    if (rightClick || cell.isFlagged) {
        rightClick = false;
        cell.isFlagged = true;
        SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
        SDL_RenderCopy(renderer, assets.flag, NULL, &rect);
        return;

    } else if (globalSettings.game_over && cell.hasMine) {
        SDL_RenderCopy(renderer, assets.mine, NULL, &rect);
        return;
    }

    else if (released || cell.isRevealed) {

        cell.isRevealed = true;
        released = false;
        
        if (cell.hasMine) {
            SDL_RenderCopy(renderer, assets.mine, NULL, &rect);
            globalSettings.game_over = true;
            return;
        }

        switch(nearbyMines) {
            case 1:
                SDL_RenderCopy(renderer, assets.prox1, NULL, &rect);
                break;
            case 2:
                SDL_RenderCopy(renderer, assets.prox2, NULL, &rect);
                break;
            case 3:
                SDL_RenderCopy(renderer, assets.prox3, NULL, &rect);
                break;
            case 4:
                SDL_RenderCopy(renderer, assets.prox4, NULL, &rect);
                break;
            case 5:
                SDL_RenderCopy(renderer, assets.prox5, NULL, &rect);
                break;
            case 6:
                SDL_RenderCopy(renderer, assets.prox6, NULL, &rect);
                break;
            case 7:
                SDL_RenderCopy(renderer, assets.prox7, NULL, &rect);
                break;
            case 8:
                SDL_RenderCopy(renderer, assets.prox8, NULL, &rect);
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
                SDL_RenderDrawRect(renderer, &rect);
                break;
        }
        return;
    }
     else if (clicked) // While click held down
    {
        SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
        SDL_RenderFillRect(renderer, &rect);
        return;
    } 

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
