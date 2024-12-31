#pragma once
#include <vector>
#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <ctime>
#include "mouseProps.hpp"
#include "settings.hpp"

struct Node {
    bool hasMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
};

struct Point {
    int x;
    int y;
};

class Draw;

std::vector<std::vector<Node>> grid();
bool cellClicked (int x, int y, int cell_x, int cell_y);
void plantMines(std::vector<std::vector<Node>> &grid, int numMines);
int checkSurrounding(std::vector<std::vector<Node>> &grid, int row, int col);
std::array<Point, 8> returnSurrounding(std::vector<std::vector<Node>> &grid, int row, int col);
void revealBlock(std::vector<std::vector<Node>> &grid, int row, int col);
void revealBlanks(std::vector<std::vector<Node>> &grid, int row, int col);
void createGrid(SDL_Renderer *renderer, std::vector<std::vector<Node>> &grid, MouseProps &mouseProps, GameAssets &assets, Draw& draw);
void clearMines(std::vector<std::vector<Node>> &grid);