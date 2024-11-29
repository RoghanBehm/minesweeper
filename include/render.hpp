
#pragma once
#include <SDL2/SDL.h>
#include "game.hpp"

constexpr int thickness = 3;
constexpr int inset = 1.5;
void draw_cell(SDL_Renderer *renderer, int x, int y, bool &clicked, bool &released, Node& cell, SDL_Texture *mine);