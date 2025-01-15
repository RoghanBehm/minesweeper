#pragma once
#include <vector>
#include <array>
#include <set>
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
    std::vector<std::vector<Node>> enemy_grid;
    int numMines;
    int safeCells;
    int revealedCells;
    std::set<std::pair<int,int>> alreadySent_;
    std::vector<std::pair<int,int>> cascadeRevealed;
    void initialize();
    void revealBlock(int row, int col, std::vector<std::pair<int,int>> &newReveals);
    std::array<Point, 8> returnSurrounding(int row, int col) const;
    void clearMines();
    void plantMines();
    std::vector<std::pair<int, int>> returnRevealed();
    bool isLegitEnemyCell(size_t row, size_t col);
    void sendNewReveals(NetworkClient &client);
public:
    Game(int rows, int cols, int numMines);
    
    void reset();
    int checkSurrounding(int row, int col) const;
    void revealBlanks(int row, int col);
    void revealEnemyCells(const std::vector<std::pair<int, int>>& coords);
    void createGrid(SDL_Renderer *renderer, NetworkClient &client, MouseProps &mouseProps, GameAssets &assets, Draw& draw);
    void createEnemyGrid(SDL_Renderer *renderer, MouseProps &mouseProps, GameAssets &assets, Draw& draw, std::vector<std::pair<int, int>> coords);
    bool checkWin();
    void revealCell(int row, int col);
};


bool cellClicked (int x, int y, int cell_x, int cell_y);
