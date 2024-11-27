#include <iostream>
#include <SDL2/SDL.h>
#include "settings.hpp"
#include "game.hpp"
#include "render.hpp"

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
    int mouseX = 0, mouseY = 0;

    bool cellIsClicked = false;
    std::vector<std::vector<Node>> mine_grid = grid();
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {   

                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
            }

            // Background (black)
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            
            for (size_t i = 0; i < mine_grid.size(); i++)
            {
                for (size_t j = 0; j < mine_grid[i].size(); ++j)
                {
                    int cell_x = j * globalSettings.cell_size;
                    int cell_y = i * globalSettings.cell_size;
                    cellIsClicked = cellClicked(mouseX, mouseY, cell_x, cell_y);
                    draw_cell(renderer, cell_x, cell_y, cellIsClicked);
                }
            }

            SDL_RenderPresent(renderer);
        }
    }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    
}