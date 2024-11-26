#include <iostream>
#include <SDL2/SDL.h>
#include "settings.hpp"

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1200, 800, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    SDL_Rect rect = {200, 150, globalSettings.cell_size, globalSettings.cell_size};

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Background (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // SQUARE
        int thickness = 2;
        // Fill colour
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        // Top side (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        SDL_Rect top = {rect.x, rect.y, rect.w, thickness};
        SDL_RenderFillRect(renderer, &top);

        // Bottom side (grey)
        SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255); // Grey
        SDL_Rect bottom = {rect.x, rect.y + rect.h - thickness, rect.w, thickness};
        SDL_RenderFillRect(renderer, &bottom);

        // Left side (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        SDL_Rect left = {rect.x, rect.y, thickness, rect.h};
        SDL_RenderFillRect(renderer, &left);

        // Right side (grey)
        SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255); // Grey
        SDL_Rect right = {rect.x + rect.w - thickness, rect.y, thickness, rect.h};
        SDL_RenderFillRect(renderer, &right);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}