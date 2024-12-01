#include <vector>
#include <iostream>
#include <ctime>
#include <SDL2/SDL.h>
#include "game.hpp"
#include "settings.hpp"

Node node {
    .hasMine = false,
    .isRevealed = false,
    .isFlagged = false,
    .adjacentMines = 0
};

std::vector<std::vector<Node>> grid() {
    int rows = 16;
    int cols = 30;
    std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));
    return grid;
}

bool cellClicked (int x, int y, int cell_x, int cell_y) {
    return (x >= cell_x && x <= cell_x + globalSettings.cell_size &&
            y >= cell_y && y <= cell_y + globalSettings.cell_size);
}

void plantMines(std::vector<std::vector<Node>> &grid, int numMines) {
    int rows = grid.size();
    int cols = grid[0].size();
    int placedMines = 0;

    std::srand(std::time(nullptr));

    while (placedMines < numMines) {
        int randomRow = std::rand() % rows;
        int randomCol = std::rand() % cols;
        if (!grid[randomRow][randomCol].hasMine) {
            grid[randomRow][randomCol].hasMine = true;

            placedMines++;
        }
        
    } 

}

int checkSurrounding(std::vector<std::vector<Node>> &grid, int row, int col) {
    int mines = 0;
    int rows = grid.size();
    int cols = grid[0].size();

    if (col + 1 < cols && grid[row][col + 1].hasMine) {
        mines++;
    }
    if (col - 1 >= 0 && grid[row][col - 1].hasMine) {
        mines++;
    }
    if (row - 1 >= 0 && grid[row - 1][col].hasMine) {
        mines++;
    }
    if (row - 1 >= 0 && col + 1 < cols && grid[row - 1][col + 1].hasMine) {
        mines++;
    }
    if (row - 1 >= 0 && col - 1 >= 0 && grid[row - 1][col - 1].hasMine) {
    }
    if (row + 1 < rows && grid[row + 1][col].hasMine) {
        mines++;
    }
    if (row + 1 < rows && col + 1 < cols && grid[row + 1][col + 1].hasMine) {
        mines++;
    }
    if (row + 1 < rows && col - 1 >= 0 && grid[row + 1][col - 1].hasMine) {
        mines++;
    }
    return mines;
}