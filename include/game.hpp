#pragma once
#include <vector>
#include <string>
#include <array>
#include <SDL2/SDL.h>
#include <ctime>
#include "mouseProps.hpp"
#include "settings.hpp"
#include "../client/client.hpp"

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

class Draw; //Forward declaration

class Game {
private:
    
    int rows;
    int cols;
    std::vector<std::vector<Node>> grid;
    int numMines;
    int safeCells;
    int revealedCells;

    void initialize();
    void revealBlock(int row, int col);
    std::array<Point, 8> returnSurrounding(int row, int col) const;
    void clearMines();
    void plantMines();
    std::vector<std::pair<int, int>> returnRevealed();
public:
    Game(int rows, int cols, int numMines);
    
    void reset();
    int checkSurrounding(int row, int col) const;
    void revealBlanks(int row, int col);
    void createGrid(SDL_Renderer *renderer, NetworkClient &client, MouseProps &mouseProps, GameAssets &assets, Draw& draw, int offset);
    bool checkWin();
    void revealCell(int row, int col);
};


bool cellClicked (int x, int y, int cell_x, int cell_y);
