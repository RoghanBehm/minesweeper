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

    struct {
        int mouseX = 0;
        int mouseY = 0;
        int mouseXr = 0;
        int mouseYr = 0;
        bool mouseDown = false;
        bool cellIsClicked = false;
        bool released = false;
    } mouseProps;

    std::vector<std::vector<Node>> mine_grid = grid();

    const int frameDelay = 1000 / 60;
    Uint32 frameStart = SDL_GetTicks();

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
                SDL_GetMouseState(&mouseProps.mouseX, &mouseProps.mouseY);
                mouseProps.mouseDown = true;
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                SDL_GetMouseState(&mouseProps.mouseXr, &mouseProps.mouseYr);
                mouseProps.mouseDown = false;
                mouseProps.released = true;
            }
            else if (event.type == SDL_MOUSEMOTION && mouseProps.mouseDown) {
                SDL_GetMouseState(&mouseProps.mouseX, &mouseProps.mouseY);
            }
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
                    mouseProps.cellIsClicked = cellClicked(mouseProps.mouseX, mouseProps.mouseY, cell_x, cell_y);
                    Node &currentCell = mine_grid[i][j];
                    mouseProps.released = cellClicked(mouseProps.mouseXr, mouseProps.mouseYr, cell_x, cell_y);
                    draw_cell(renderer, cell_x, cell_y, mouseProps.cellIsClicked, mouseProps.released, currentCell);
                }
            }


            SDL_RenderPresent(renderer);
            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
    }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    
}