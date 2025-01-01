
#pragma once
#include <SDL2/SDL.h>
#include "game.hpp"
#include "settings.hpp"
#include "render.hpp"

constexpr int thickness = 3;
constexpr int inset = 1.5;

struct Node;

class Draw {
    
private:
    void default_cell(SDL_Renderer *renderer, SDL_Rect rect);
    void mine_prox_cell(SDL_Renderer *renderer, GameAssets &assets, int nearbyMines, SDL_Rect rect);

public:
    void cell(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released, Node &cell, Game &game, GameAssets &assets, int nearbyMines, int row, int col);
    void menu(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released);

};