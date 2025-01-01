#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "settings.hpp"
#include "game.hpp"
#include "render.hpp"
#include "mouseProps.hpp"

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          900, 800, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize game objects
    Game game(16, 30, 1);
    Draw draw;
    GameAssets assets;
    MouseProps mouseProps;

    // Load assets
    if (loadGameAssets(renderer, &assets) != 0) {
        printf("Failed to load game assets.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Gloop
    bool running = true;
    SDL_Event event;
    const int frameDelay = 1000 / 60;
    Uint32 frameStart = SDL_GetTicks();


    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (!globalSettings.game_over) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
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
                } else if (event.type == SDL_MOUSEMOTION && mouseProps.mouseDown) {
                    SDL_GetMouseState(&mouseProps.mouseX, &mouseProps.mouseY);
                }
            }
        }

        // Draw background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Menu input
        int reset_x = globalSettings.menu_width / 2 - globalSettings.reset_button_width / 2;
        int reset_y = globalSettings.menu_height / 2 - globalSettings.reset_button_height / 2;
        bool reset_released = cellClicked(mouseProps.mouseXr, mouseProps.mouseYr, reset_x, reset_y);
        bool reset_clicked = cellClicked(mouseProps.mouseX, mouseProps.mouseY, reset_x, reset_y);

        // Reset game if regenerate
        if (globalSettings.regenerate) {
            game.reset();
            globalSettings.regenerate = false;
            globalSettings.first_click = true;
        }
        

        // Render menu
        draw.menu(renderer, reset_x, reset_y, reset_clicked, reset_released);

        // Render game grid
        game.createGrid(renderer, mouseProps, assets, draw);
        if (game.checkWin()) {
            std::cout << "winnnnnna";
        } else {
            std::cout << "no";
        }

        SDL_RenderPresent(renderer);
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
