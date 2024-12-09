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
            else if (!globalSettings.game_over) {
                if (event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        if (event.button.button == SDL_BUTTON_RIGHT) {
                            SDL_GetMouseState(&mouseProps.mouseXc, &mouseProps.mouseYc);
                            mouseProps.rightClicked = true;
                        } else {
                            SDL_GetMouseState(&mouseProps.mouseX, &mouseProps.mouseY);
                            mouseProps.mouseDown = true;
                        }
                        
                    } else if (event.type == SDL_MOUSEBUTTONUP) {
                        if (event.button.button == SDL_BUTTON_RIGHT) {
                            mouseProps.rightClicked = false; 
                        } else {
                            SDL_GetMouseState(&mouseProps.mouseXr, &mouseProps.mouseYr);
                            mouseProps.mouseDown = false;
                            mouseProps.released = true;
                        }
                    }
                    else if (event.type == SDL_MOUSEMOTION && mouseProps.mouseDown) {
                        SDL_GetMouseState(&mouseProps.mouseX, &mouseProps.mouseY);
                    }
            }
            
        }
            int reset_x = globalSettings.menu_width / 2 - globalSettings.reset_button_width / 2;
            bool reset_y = globalSettings.menu_height / 2 - globalSettings.reset_button_height / 2;
            bool reset_released = cellClicked(mouseProps.mouseXr, mouseProps.mouseYr, reset_x, reset_y);
            bool reset_clicked = cellClicked(mouseProps.mouseX, mouseProps.mouseY, reset_x, reset_y);
            
            // Reset game if regenerate
            if (globalSettings.regenerate) {
                    clearMines(mine_grid);
                    mine_grid = grid();
                    plantMines(mine_grid, 100);
                    globalSettings.regenerate = false;
                    globalSettings.first_click = true;
            }
            draw_menu(renderer, reset_x, reset_y, reset_clicked, reset_released);
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