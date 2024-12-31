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

class Draw; //Forward-declaration

class Game {
private:
    
    int rows;
    int cols;
    std::vector<std::vector<Node>> grid;
    int numMines;
public:
    Game(int rows, int cols, int numMines); // Constructor

    // Grid Operations
    void plantMines();
    void clearMines();
    int checkSurrounding(int row, int col) const;
    std::array<Point, 8> returnSurrounding(int row, int col) const;
    void revealBlock(int row, int col);
    void revealBlanks(int row, int col);
    void createGrid(SDL_Renderer *renderer, MouseProps &mouseProps, GameAssets &assets, Draw& draw);

    // Accessors
    const std::vector<std::vector<Node>>& getGrid() const;

};


bool cellClicked (int x, int y, int cell_x, int cell_y);
