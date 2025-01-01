#include <vector>
#include <iostream>



#include "game.hpp"
#include "render.hpp"

Node node {
    .hasMine = false,
    .isRevealed = false,
    .isFlagged = false,
    .adjacentMines = 0,
    };


bool cellClicked(int x, int y, int cell_x, int cell_y)
{
    return (x >= cell_x && x <= cell_x + globalSettings.cell_size &&
            y >= cell_y && y <= cell_y + globalSettings.cell_size);
}
void Game::reset() {

    initialize();

}

void Game::initialize() {

    grid = std::vector<std::vector<Node>>(rows, std::vector<Node>(cols));
    globalSettings.game_over = false;
    plantMines();

}

Game::Game(int rows, int cols, int numMines)
    : rows(rows), cols(cols), numMines(numMines) {
    std::srand(std::time(nullptr));
    initialize();
    safeCells = rows * cols - numMines;
    revealedCells = 0;
}

void Game::clearMines()
{
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j].hasMine) {
                grid[i][j].hasMine = false;
            }
        }
    }
}

void Game::plantMines()
{

    int placedMines = 0;

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

int Game::checkSurrounding(int row, int col) const
{
    int mines = 0;

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


    
std::array<Point, 8> Game::returnSurrounding(int row, int col) const {
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

void Game::revealBlock(int row, int col) {

    const int directions[8][2] = {
        {-1, 0},  {1, 0},  {0, -1},  {0, 1},
        {-1, -1}, {-1, 1}, {1, -1},  {1, 1}
    };

    if (!grid[row][col].isRevealed) {
        grid[row][col].isRevealed = true;
        revealedCells++;
    }
    

    for (const auto &dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            if (!grid[newRow][newCol].isRevealed) {
                grid[newRow][newCol].isRevealed = true;
                revealedCells++; 
            }
        }
    }
}


bool Game::checkWin() {
    if (safeCells == revealedCells) {
        return true;
    }
    return false;
}

void Game::revealBlanks(int row, int col) {
    // If there are no surrounding mines, reveal neighbors and cascade until no safe mines left
    if (!checkSurrounding(row, col)){
        revealBlock(row, col);

        std::array<Point, 8> surrounding = returnSurrounding(row, col);
        for (const auto &point : surrounding) {
            if (point.x >= 0 && point.x < static_cast<int>(grid.size()) &&
                point.y >= 0 && point.y < static_cast<int>(grid[0].size()) &&
                !grid[point.x][point.y].isRevealed) {
                revealBlanks(point.x, point.y);
            }
        }
    }
}

void Game::revealCell(int row, int col) {
    if (!grid[row][col].isRevealed) {
        grid[row][col].isRevealed = true;
        revealedCells++;

        if (grid[row][col].hasMine) {
            globalSettings.game_over = true;
        }
    }
}


void Game::createGrid(SDL_Renderer *renderer, MouseProps &mouseProps, GameAssets &assets, Draw& draw){
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            int cell_x = j * globalSettings.cell_size;
            int cell_y = i * globalSettings.cell_size + globalSettings.menu_height;

            // Handle right-click toggle
            
            if (cellClicked(mouseProps.mouseXc, mouseProps.mouseYc, cell_x, cell_y) && mouseProps.rightClicked) {
                if (!globalSettings.game_over && !grid[i][j].isRevealed) {
                    grid[i][j].isFlagged = !grid[i][j].isFlagged;
                    mouseProps.rightClicked = false;
                }
                
            }
            mouseProps.cellIsClicked = cellClicked(mouseProps.mouseX, mouseProps.mouseY, cell_x, cell_y);
            // Pass current cell to cell for rendering
            Node &currentCell = grid[i][j];
            mouseProps.released = cellClicked(mouseProps.mouseXr, mouseProps.mouseYr, cell_x, cell_y);
            int surroundingMines = checkSurrounding(i, j);
            draw.cell(renderer, cell_x, cell_y, mouseProps.cellIsClicked, mouseProps.released, currentCell, *this, assets, surroundingMines, i, j);


            // If current cell does not contain mine, reveal neighbouring cells if none contain mines
            if (currentCell.isRevealed && !currentCell.hasMine && !globalSettings.regenerate) {
                revealBlanks(i, j);
            }
        }
    }
}

