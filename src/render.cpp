#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "render.hpp"
#include "settings.hpp"
#include "game.hpp"

void draw_cell(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released, Node& cell, GameAssets &assets, int nearbyMines)
{
    SDL_Rect rect = {x, y, globalSettings.cell_size, globalSettings.cell_size};

    if (cell.isFlagged) {
        SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
        SDL_RenderCopy(renderer, assets.flag, NULL, &rect);
        return;
    }

    if ((globalSettings.game_over && cell.hasMine) && !globalSettings.regenerate) {
        if (released) {
            SDL_RenderCopy(renderer, assets.clicked_mine, NULL, &rect);
            return;
        }
        SDL_RenderCopy(renderer, assets.mine, NULL, &rect);
        return;
    }

    else if (released || cell.isRevealed) {
        
        cell.isRevealed = true;
        released = false;
        
        if (cell.hasMine && !globalSettings.regenerate) {
            if (globalSettings.first_click) {
                cell.hasMine = false;
                globalSettings.regenerate = true;
                globalSettings.first_click = false;
                return;
            }
            SDL_RenderCopy(renderer, assets.mine, NULL, &rect);
            globalSettings.game_over = true;
            return;
        }
        globalSettings.first_click = false;

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

void draw_menu(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released) {

    // Menu
    SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
    SDL_Rect menu = {0, 0, globalSettings.menu_width, globalSettings.menu_height};
    SDL_RenderFillRect(renderer, &menu);

    // Reset button

    if (!clicked) {
        SDL_SetRenderDrawColor(renderer, 0, 10, 103, 0);
    } else {
        SDL_SetRenderDrawColor(renderer, 10, 120, 13, 40);
    }
    
    SDL_Rect reset_button = {
        x,
        y,
        globalSettings.reset_button_height,
        globalSettings.reset_button_width
    };
    SDL_RenderFillRect(renderer, &reset_button);

    if (released) {
        globalSettings.regenerate = true;
        SDL_SetRenderDrawColor(renderer, 0, 10, 103, 0);
        SDL_RenderFillRect(renderer, &reset_button);
    }

            
}
