
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "game.hpp"
#include "settings.hpp"


constexpr int thickness = 3;
constexpr int inset = 1;

struct Node;

class Draw {
     
private:
    void default_cell(SDL_Renderer *renderer, SDL_Rect rect);
    void mine_prox_cell(SDL_Renderer *renderer, GameAssets &assets, int nearbyMines, SDL_Rect rect);

public:
    void cell(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released, Node &cell, Game &game, GameAssets &assets, int nearbyMines, int row, int col);
    void menu(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released);
    void blackFilter(SDL_Renderer *renderer);
    void Popup(SDL_Renderer *renderer, TTF_Font *font, const char *message);

};