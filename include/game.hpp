#pragma once
#include <vector>
#include <string>
#include <array>

#include <ctime>

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


std::vector<std::vector<Node>> grid();
bool cellClicked (int x, int y, int cell_x, int cell_y);
void plantMines(std::vector<std::vector<Node>> &grid, int numMines);
int checkSurrounding(std::vector<std::vector<Node>> &grid, int row, int col);
std::array<Point, 8> returnSurrounding(std::vector<std::vector<Node>> &grid, int row, int col);
void revealBlock(std::vector<std::vector<Node>> &grid, int row, int col);
void revealBlanks(std::vector<std::vector<Node>> &grid, int row, int col);