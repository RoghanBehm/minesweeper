#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "settings.hpp"
#include "game.hpp"
#include "render.hpp"
#include "mouseProps.hpp"

int main()
{
    std::srand(std::time(nullptr));

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          900, 800, SDL_WINDOW_SHOWN);
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

    MouseProps mouseProps;
    std::vector<std::vector<Node>> mine_grid = grid();

    const int frameDelay = 1000 / 60;
    Uint32 frameStart = SDL_GetTicks();
    plantMines(mine_grid, 100);
    GameAssets assets;

    if (loadGameAssets(renderer, &assets) != 0) {
        printf("Failed to load game assets.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

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
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    SDL_GetMouseState(&mouseProps.mouseXc, &mouseProps.mouseYc);
                } else {
                    SDL_GetMouseState(&mouseProps.mouseX, &mouseProps.mouseY);
                    mouseProps.mouseDown = true;
                }
                
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
            // Menu
            SDL_SetRenderDrawColor(renderer, 123, 123, 123, 255);
            SDL_Rect menu = {0, 0, globalSettings.menu_width, globalSettings.menu_height};
            SDL_RenderFillRect(renderer, &menu);


            if (globalSettings.regenerate) {
                    clearMines(mine_grid);
                    mine_grid = grid();
                    plantMines(mine_grid, 100);
                    globalSettings.regenerate = false;
                    globalSettings.first_click = true;
            }

            createGrid(renderer, mine_grid, mouseProps, assets);

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