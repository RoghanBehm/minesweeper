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