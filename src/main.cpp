#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_image.h>
#include "settings.hpp"
#include "game.hpp"
#include "render.hpp"
#include "mouseProps.hpp"
#include <array>
#include <string>
#include <boost/asio.hpp>
#include "../client/client.hpp"

int main(int argc, char*argv[]) {
    if (argc != 3) {
        std::cout << "Incorrect usage: ./minesweeper <IP_Address> <Port>" << std::endl;
        return 0;
    }
    std::cout << argv[1] << argv[2] << std::endl;

    boost::asio::io_context io_context;
    NetworkClient client(io_context, argv[1], argv[2]);
    std::thread io_thread([&io_context]() { io_context.run(); });

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          globalSettings.window_width,
                                          globalSettings.window_height,
                                          SDL_WINDOW_SHOWN);
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

    SDL_RenderSetLogicalSize(renderer, globalSettings.window_width, globalSettings.window_height);

    if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
        std::cerr << "SDL_SetWindowFullscreen Error: " << SDL_GetError() << std::endl;
    }

    // Initialize game objects
    while (!globalSettings.seed_received) {};
    Game game(16, 30, 100);
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

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }


    std::string basePath(SDL_GetBasePath());
    std::string fontPath = basePath + "../fonts/Helvetica.ttf";

    TTF_Font *font = TTF_OpenFont(fontPath.c_str(), 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_f) {
                    Uint32 fullscreen_flag = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
                    SDL_SetWindowFullscreen(window, fullscreen_flag ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            } else {
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
        
        auto board = client.return_board();

        // Render menu
        draw.menu(renderer, reset_x, reset_y, reset_clicked, reset_released);

        // Render player grids
        game.createGrid(renderer, client, mouseProps, assets, draw);
        game.createEnemyGrid(renderer, mouseProps, assets, draw, board);

        if (game.checkWin()) {
            draw.blackFilter(renderer);
            draw.Popup(renderer, font, "You wonnered!");
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
    TTF_Quit();
    SDL_Quit();

    return 0;
}
