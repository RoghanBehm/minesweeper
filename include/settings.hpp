#pragma once
#include <SDL2/SDL.h>
#include <SDL_image.h>
struct Settings {
    int cell_size;
    int menu_height;
    int menu_width;
    bool game_over;
    bool first_click;
    bool regenerate;
};

extern Settings globalSettings;
typedef struct {
    SDL_Texture *mine;
    SDL_Texture *flag;
    SDL_Texture *prox1;
    SDL_Texture *prox2;
    SDL_Texture *prox3;
    SDL_Texture *prox4;
    SDL_Texture *prox5;
    SDL_Texture *prox6;
    SDL_Texture *prox7;
    SDL_Texture *prox8;

} GameAssets;
int loadGameAssets(SDL_Renderer *renderer, GameAssets *assets);