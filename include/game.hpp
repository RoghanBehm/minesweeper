#include <vector>

struct Node {
    bool hasMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
};

std::vector<std::vector<Node>> grid();