#include <vector>
#include <iostream>
#include "game.hpp"

Node node {
    .hasMine = false,
    .isRevealed = false,
    .isFlagged = false,
    .adjacentMines = 0
};

std::vector<std::vector<Node>> grid() {
    int rows = 5;
    int cols = 5;
    std::vector<std::vector<Node>> grid(rows, std::vector<Node>(cols));
    return grid;
}

