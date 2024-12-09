
#pragma once
#include <SDL2/SDL.h>
#include "game.hpp"
#include "settings.hpp"

constexpr int thickness = 3;
constexpr int inset = 1.5;
void draw_cell(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released, Node& cell, GameAssets &assets, int nearbyMines);
void draw_menu(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released);