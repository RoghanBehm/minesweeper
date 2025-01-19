#include <vector>
#include <array>
#include "game.hpp"
#include "render.hpp"
#include "serialize.hpp"


Node node {
    .hasMine = false,
    .isRevealed = false,
    .isFlagged = false,
    .exploded = false,
    .adjacentMines = 0,
    };


bool cellClicked(int x, int y, int cell_x, int cell_y)
{
    return (x >= cell_x && x <= cell_x + globalSettings.cell_size &&
            y >= cell_y && y <= cell_y + globalSettings.cell_size);
}

Game::Game(int rows, int cols, int numMines)
    : rows(rows), cols(cols), numMines(numMines) {
    initialize();
    safeCells = rows * cols - numMines;
    revealedCells = 0;
    popupActive = false;
}

void Game::reset() {

    alreadySent_.clear();
    initialize();

}

void Game::initialize() {

    grid = std::vector<std::vector<Node>>(rows, std::vector<Node>(cols));
    enemy_grid = std::vector<std::vector<Node>>(rows, std::vector<Node>(cols));
    globalSettings.game_over = false;
    plantMines();

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

void Game::setExploded(int cell_x, int cell_y) 
{ 
        for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j].exploded) {
                return;
            }
        }
    } grid[cell_x][cell_y].exploded = true;
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

void Game::revealBlock(int row, int col, std::vector<std::pair<int,int>> &newReveals) {

    const int directions[8][2] = {
        {-1, 0},  {1, 0},  {0, -1},  {0, 1},
        {-1, -1}, {-1, 1}, {1, -1},  {1, 1}
    };

    if (!grid[row][col].isRevealed) {
        grid[row][col].isRevealed = true;
        revealedCells++;
        newReveals.emplace_back(row, col);
    }
    

    for (const auto &dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            if (!grid[newRow][newCol].isRevealed) {
                grid[newRow][newCol].isRevealed = true;
                revealedCells++; 
                newReveals.emplace_back(newRow, newCol);
            }
        }
    }
}


bool Game::checkWin() {
    if (revealedCells == (rows * cols - numMines)) {
        popupActive = true;
        win = true;
        return true;
    }
    return false;
}

void Game::revealBlanks(int row, int col) {
    // If there are no surrounding mines, reveal neighbors and cascade until no safe mines left
    if (!checkSurrounding(row, col)){
        revealBlock(row, col, cascadeRevealed);

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
            popupActive = true;
        }
    }
}

bool Game::isLegitEnemyCell(size_t row, size_t col) {
    return row < enemy_grid.size() && col < enemy_grid[row].size();
}




void Game::revealEnemyCells(const std::vector<std::pair<int, int>>& coords) {
    for (auto [x, y] : coords) {
        if (isLegitEnemyCell(x, y)) {
            enemy_grid[x][y].isRevealed = true;
        }
    }
}


std::vector<std::pair<int, int>> Game::returnRevealed() {
    std::vector<std::pair<int, int>> revealedCoordinates;

    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j].isRevealed) {
                revealedCoordinates.emplace_back(i, j);
            }
        }
    }

    return revealedCoordinates;
}


void Game::sendNewReveals(NetworkClient &client)
{
    auto allRevealed = returnRevealed();

    std::vector<std::pair<int, int>> newlyRevealed;
    for (auto &rc : allRevealed) {
        if (!alreadySent_.count(rc)) {
            newlyRevealed.push_back(rc);
            alreadySent_.insert(rc);
        }
    }
    std::vector<std::pair<int, int>> combined;
    combined.reserve(newlyRevealed.size() + cascadeRevealed.size());

    combined.insert(combined.end(), newlyRevealed.begin(), newlyRevealed.end());

    for (auto &rc : cascadeRevealed) {
        if (!alreadySent_.count(rc)) {
            combined.push_back(rc);
            alreadySent_.insert(rc);
        }
    }

    cascadeRevealed.clear();

    if (!combined.empty()) {
        auto serialized = serialize_pairs(combined);
        client.send_message(serialized);
    }
}

void Game::sendWin(NetworkClient &client)
{
    auto serialized = serialize_bool(true);
    client.send_message(serialized);
}

void Game::createGrid(SDL_Renderer *renderer, NetworkClient &client, MouseProps &mouseProps, const GameAssets &assets, Draw& draw)
{
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

            // Pass current cell to draw.cell for rendering
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
    if (globalSettings.seed_received) 
    {
        sendNewReveals(client);
    }
}


void Game::createEnemyGrid(SDL_Renderer *renderer, MouseProps &mouseProps,const GameAssets &assets, Draw& draw, std::vector<std::pair<int, int>> coords)
{

    revealEnemyCells(coords);
    for (size_t i = 0; i < enemy_grid.size(); i++) {
        for (size_t j = 0; j < enemy_grid[i].size(); ++j) {
            int offset = 960;
            int cell_x = j * globalSettings.cell_size + offset;
            int cell_y = i * globalSettings.cell_size + globalSettings.menu_height;            

            // Pass current cell to cell for rendering
            Node &currentCell = enemy_grid[i][j];
            int surroundingMines = 0; // Disable mine check for enemy grid
            
            draw.cell(renderer, cell_x, cell_y, mouseProps.cellIsClicked, mouseProps.released, currentCell, *this, assets, surroundingMines, i, j);
        }
    }
}