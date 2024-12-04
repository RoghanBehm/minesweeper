#include <vector>
#include <iostream>

#include <SDL2/SDL.h>

#include "game.hpp"
#include "settings.hpp"

Node node {
    .hasMine = false,
    .isRevealed = false,
    .isFlagged = false,
    .adjacentMines = 0
    };

std::vector<std::vector<Node>> grid()
{
    int rows = 16;
    int cols = 30;
    std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));
    return grid;
}

bool cellClicked(int x, int y, int cell_x, int cell_y)
{
    return (x >= cell_x && x <= cell_x + globalSettings.cell_size &&
            y >= cell_y && y <= cell_y + globalSettings.cell_size);
}

void plantMines(std::vector<std::vector<Node>> &grid, int numMines)
{
    int rows = grid.size();
    int cols = grid[0].size();
    int placedMines = 0;

    std::srand(std::time(nullptr));

    while (placedMines < numMines)
    {
        int randomRow = std::rand() % rows;
        int randomCol = std::rand() % cols;
        if (!grid[randomRow][randomCol].hasMine)
        {
            grid[randomRow][randomCol].hasMine = true;

            placedMines++;
        }
    }
}

int checkSurrounding(std::vector<std::vector<Node>> &grid, int row, int col)
{
    int mines = 0;
    int rows = grid.size();
    int cols = grid[0].size();

    const int directions[8][2] = {
        {-1, 0},  // Up
        {1, 0},   // Down
        {0, -1},  // Left
        {0, 1},   // Right
        {-1, -1}, // Up-Left
        {-1, 1},  // Up-Right
        {1, -1},  // Down-Left
        {1, 1}    // Down-Right
    };

    for (const auto &dir : directions)
    {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        
        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols)
        {
            if (grid[newRow][newCol].hasMine)
            {
                mines++;
            }
        }
    }
    return mines;
}


    
std::array<Point, 8> returnSurrounding(std::vector<std::vector<Node>> &grid, int row, int col) {
    std::array<Point, 8> nearby = {};
    int i = 0;

    const int directions[8][2] = {
        {-1, 0},  {1, 0},  {0, -1},  {0, 1},
        {-1, -1}, {-1, 1}, {1, -1},  {1, 1}
    };

    for (const auto &dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < static_cast<int>(grid.size()) &&
            newCol >= 0 && newCol < static_cast<int>(grid[0].size())) {
            nearby[i] = {newRow, newCol};
            i++;
        }
    }

    for (; i < 8; i++) {
        nearby[i] = {-1, -1};
    }

    return nearby;
}

void revealBlock(std::vector<std::vector<Node>> &grid, int row, int col) {
    int rows = grid.size();
    int cols = grid[0].size();

    const int directions[8][2] = {
        {-1, 0},  {1, 0},  {0, -1},  {0, 1},
        {-1, -1}, {-1, 1}, {1, -1},  {1, 1}
    };

    grid[row][col].isRevealed = true;

    for (const auto &dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            grid[newRow][newCol].isRevealed = true;
        }
    }
}

void revealBlanks(std::vector<std::vector<Node>> &grid, int row, int col) {
    // If there are no surrounding mines, reveal neighbors and cascade until no safe mines left
    if (!checkSurrounding(grid, row, col)){
        revealBlock(grid, row, col);

        std::array<Point, 8> surrounding = returnSurrounding(grid, row, col);
        for (const auto &point : surrounding) {
            if (point.x >= 0 && point.x < static_cast<int>(grid.size()) &&
                point.y >= 0 && point.y < static_cast<int>(grid[0].size()) &&
                !grid[point.x][point.y].isRevealed) {
                revealBlanks(grid, point.x, point.y);
            }
        }
    }
}

