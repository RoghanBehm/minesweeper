#pragma once
#include <vector>

struct Node {
    bool hasMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
};

std::vector<std::vector<Node>> grid();
bool cellClicked (int x, int y, int cell_x, int cell_y);
void plantMines(std::vector<std::vector<Node>> &grid, int numMines);