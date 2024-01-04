#include "level.h"
#include <vector>
#include "collision.h"
#include <algorithm>
using std::abs;
using std::cout;
using std::sort;
using std::string;
using std::vector;

Level::Level(int n = 1) : n(n)
{
    ReadBoardFile();
    addObstaclesToVector();
}

void Level::addObstaclesToVector()
{
    SDL_Rect block;
    block.w = Params::kCellWidth;
    block.h = Params::kCellHeight;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            if (board[i][j] == State::kObstacle)
            {
                block.x = j * block.w;
                block.y = i * block.h;
                obstacles.emplace_back(block);
            }
        }
    }
}

bool Level::obsCollWithHead(SDL_Rect &head)
{
    for (auto &obst : obstacles)
    {
        if (Collision::check_collision(obst, head))
        {
            return true;
        }
    }
    return false;
}

std::vector<Level::State> Level::ParseLine(const std::string &line)
{
    std::istringstream sline(line);
    int n;
    char c;
    std::vector<State> row;
    while (sline >> n >> c && c == ',')
    {
        if (n == 0)
        {
            row.emplace_back(State::kEmpty);
        }
        if (n == 1)
        {
            row.emplace_back(State::kObstacle);
        }
    }
    return row;
}

void Level::ReadBoardFile()
{
    board.clear();
    std::string path = "../src/level" + std::to_string(n) + ".txt";

    std::ifstream myfile(path);
    if (myfile)
    {
        std::string line;
        while (getline(myfile, line))
        {
            std::vector<State> row = ParseLine(line);
            board.emplace_back(row);
        }
    }
    else
    {
        std::cout << "Cannot read file from path: " << path << std::endl;
    }
}

bool Level::Compare(const vector<int> &a, const vector<int> &b)
{
    int f1 = a[2] + a[3]; // f1 = g1 + h1
    int f2 = b[2] + b[3]; // f2 = g2 + h2
    return f1 > f2;
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void Level::CellSort(vector<vector<int>> *v)
{
    sort(v->begin(), v->end(), Level::Compare);
    // v->erase(unique(v->begin(), v->end(), Level::CompareIfEquals), v->end());
}

// Calculate the manhattan distance
int Level::Heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x2 - x1) + abs(y2 - y1);
}

/**
 * Check that a cell is valid: on the board, not an obstacle, and clear.
 */
bool Level::CheckValidCell(int x, int y)
{
    bool on_board_x = (x >= 0 && x < board.size());
    bool on_board_y = (y >= 0 && y < board[0].size());
    if (on_board_x && on_board_y)
        return board[x][y] == State::kEmpty;
    return false;
}

bool Level::CheckValidCell2(int x, int y)
{
    bool on_board_x = (x >= 0 && x < board.size());
    bool on_board_y = (y >= 0 && y < board[0].size());
    if (on_board_x && on_board_y)
        return board[x][y] == State::kPath || board[x][y] == State::kStart;
    return false;
}

/**
 * Add a node to the open list and mark it as open.
 */
void Level::AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist)
{
    // Add node to open vector, and mark board cell as closed.
    openlist.emplace_back(vector<int>{x, y, g, h});
    board[x][y] = State::kClosed;
}

/**
 * Expand current nodes's neighbours and add them to the open list.
 */
void Level::ExpandNeighbours(const vector<int> &current, int goal[2], vector<vector<int>> &openlist)
{
    // Get current node's data.
    int x = current[0];
    int y = current[1];
    int g = current[2];

    // Loop through current node's potential neighbours.
    for (int i = 0; i < 4; i++)
    {
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];

        // Check that the potential neighbour's x2 and y2 values are on the board and not closed.
        if (CheckValidCell(x2, y2))
        {
            // Increment g value and add neighbour to open list.
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist);
        }
    }
}

void Level::addPathToVector()
{
    SDL_Rect block;
    block.w = Params::kCellWidth;
    block.h = Params::kCellHeight;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            if (board[i][j] == State::kPath2 || board[i][j] == State::kFinish)
            {
                block.x = j * block.w;
                block.y = i * block.h;
                path.emplace_back(block);
            }
        }
    }
}

bool Level::Search(int initX, int initY, int goalX, int goalY)
{
    ReadBoardFile();
    int init[2] = {initX, initY};
    int goal[2] = {goalX, goalY};
    // Create the vector of open nodes.
    vector<vector<int>> open{};

    // Initialize the starting node.
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0], goal[1]);
    AddToOpen(x, y, g, h, open);

    while (open.size() > 0)
    {
        // Get the next node
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        board[x][y] = State::kPath;

        // Check if we're done.
        if (x == goal[0] && y == goal[1])
        {
            board[init[0]][init[1]] = State::kStart;
            board[goal[0]][goal[1]] = State::kFinish;
            return true;
        }
        // If we're not done, expand search to current node's neighbours.
        ExpandNeighbours(current, goal, open);
    }
    std::cout << "No path found!" << std::endl;
    ReadBoardFile();
    return false;
}

// Search2 improve ASearch method, becouse ASearch method is not sufficient to find the correct path
void Level::Search2(int initX, int initY, int goalX, int goalY)
{
    int init[2] = {goalX, goalY};
    int goal[2] = {initX, initY};
    int x = init[0];
    int y = init[1];
    int xSurrounding = 0;
    int ySurrounding = 0;
    int directionCurrent[2] = {0, 0};
    int directionPrevious[2] = {0, 0};
    int directionX = 0;
    int directionY = 0;
    int directionGoalX = 0;
    int directionGoalY = 0;
    std::vector<int> neighbourCloserToGoal = {};
    std::vector<int> neighbourNotCloserToGoal = {};
    std::vector<int> neighbourTemp = {};
    bool backToCheckpoint = false;
    std::vector<std::vector<int>> neighbours = {};
    std::vector<std::vector<int>> checkpoints = {};
    board[x][y] = State::kPath2;

    while (1)
    {
        // 1. check neighbours
        for (int i = 0; i < 4; i++)
        {
            xSurrounding = x + delta[i][0];
            ySurrounding = y + delta[i][1];
            if (CheckValidCell2(xSurrounding, ySurrounding))
                neighbours.emplace_back(std::vector<int>{xSurrounding, ySurrounding});
        }
        if (neighbours.empty())
        {
            auto ch = checkpoints.back();
            x = ch[0];
            y = ch[1];
            if (CheckValidCell2(x + ch[2] * (-1), y + ch[3] * (-1)))
            {
                directionCurrent[0] = ch[2] * (-1);
                directionCurrent[1] = ch[3] * (-1);
            }
            else
            {
                directionCurrent[0] = ch[2];
                directionCurrent[1] = ch[3];
            }
            checkpoints.pop_back();
            backToCheckpoint = true;
            for (int i = 0; i < board.size(); i++)
            {
                for (int j = 0; j < board[i].size(); j++)
                {
                    if (board[i][j] == State::kTempPath)
                        board[i][j] = State::kClosed;
                }
            }
        }
        // 2. change direction, if there is a need
        if ((directionCurrent[0] == 0 && directionCurrent[1] == 0) || !backToCheckpoint)
        {
            for (auto &n : neighbours)
            {
                (n[0] - x != 0) ? directionX = (n[0] - x) / abs(n[0] - x) : directionX = 0;
                (n[1] - y != 0) ? directionY = (n[1] - y) / abs(n[1] - y) : directionY = 0;
                (goal[0] - x != 0) ? directionGoalX = (goal[0] - x) / abs(goal[0] - x) : directionGoalX = 0;
                (goal[1] - y != 0) ? directionGoalY = (goal[1] - y) / abs(goal[1] - y) : directionGoalY = 0;

                if ((directionX == directionGoalX && directionX != 0 && directionGoalX != 0) ||
                    (directionY == directionGoalY && directionY != 0 && directionGoalY != 0))
                    neighbourCloserToGoal = n;
                else
                    neighbourNotCloserToGoal = n;
            }

            neighbourCloserToGoal.empty() ? neighbourTemp = neighbourNotCloserToGoal : neighbourTemp = neighbourCloserToGoal;
            (neighbourTemp[0] - x != 0) ? directionX = (neighbourTemp[0] - x) / abs(neighbourTemp[0] - x) : directionX = 0;
            (neighbourTemp[1] - y != 0) ? directionY = (neighbourTemp[1] - y) / abs(neighbourTemp[1] - y) : directionY = 0;
            (goal[0] - x != 0) ? directionGoalX = (goal[0] - x) / abs(goal[0] - x) : directionGoalX = 0;
            (goal[1] - y != 0) ? directionGoalY = (goal[1] - y) / abs(goal[1] - y) : directionGoalY = 0;

            if (directionX == directionGoalX && directionX != 0 && directionGoalX != 0)
            {
                directionCurrent[0] = directionX;
                directionCurrent[1] = 0;
            }
            else if (directionY == directionGoalY && directionY != 0 && directionGoalY != 0)
            {
                directionCurrent[0] = 0;
                directionCurrent[1] = directionY;
            }
            else if (directionX != 0)
            {
                directionCurrent[0] = directionX;
                directionCurrent[1] = 0;
            }
            else if (directionY != 0)
            {
                directionCurrent[0] = 0;
                directionCurrent[1] = directionY;
            }
            if (directionPrevious[0] != directionCurrent[0] || directionPrevious[1] != directionCurrent[1])
            {
                checkpoints.emplace_back(std::vector<int>{x, y, directionCurrent[0], directionCurrent[1]});
                for (int i = 0; i < board.size(); i++)
                {
                    for (int j = 0; j < board[i].size(); j++)
                    {
                        if (board[i][j] == State::kTempPath)
                            board[i][j] = State::kPath2;
                    }
                }
            }
        }
        // 3. move to the next cell
        x += directionCurrent[0];
        y += directionCurrent[1];
        directionPrevious[0] = directionCurrent[0];
        directionPrevious[1] = directionCurrent[1];
        board[x][y] = State::kTempPath;
        // clean up
        neighbours.clear();
        neighbourTemp.clear();
        neighbourNotCloserToGoal.clear();
        neighbourCloserToGoal.clear();
        backToCheckpoint = false;
        if (x == goal[0] && y == goal[1])
        {
            for (int i = 0; i < board.size(); i++)
            {
                for (int j = 0; j < board[i].size(); j++)
                {
                    if (board[i][j] == State::kTempPath)
                        board[i][j] = State::kPath2;
                }
            }
            return;
        }
    }
}

void Level::updatePath(int initX, int initY, int goalX, int goalY)
{
    bool foundPath = Search(initX, initY, goalX, goalY);
    if (foundPath)
    {
        Search2(initX, initY, goalX, goalY);
        addPathToVector();
    }
}

string Level::CellString(Level::State cell)
{
    switch (cell)
    {
    case State::kObstacle:
        return "B   ";
    case State::kPath2:
        return "P   ";
    case State::kStart:
        return "S   ";
    case State::kFinish:
        return "F   ";
    default:
        return "0   ";
    }
}

void Level::PrintBoard()
{
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
    cout << "-----------------------------------------------------------------------------------------------------------------" << std::endl;
}