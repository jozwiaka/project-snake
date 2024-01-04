#pragma once
#include <algorithm> // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <SDL.h>
#include "params.h"

class Level
{
public:
    enum class State
    {
        kEmpty,
        kObstacle,
        kClosed,
        kPath,
        kPath2,
        kStart,
        kFinish,
        kTempPath
    };
    Level(int);
    void updatePath(int, int, int, int);
    void PrintBoard();
    bool obsCollWithHead(SDL_Rect &);

private:
    void ReadBoardFile();
    std::vector<State> ParseLine(const std::string &);
    bool static Compare(const std::vector<int> &, const std::vector<int> &);
    void CellSort(std::vector<std::vector<int>> *);
    int Heuristic(int, int, int, int);
    bool CheckValidCell(int, int);
    bool CheckValidCell2(int, int);
    void AddToOpen(int, int, int, int, std::vector<std::vector<int>> &);
    void ExpandNeighbours(const std::vector<int> &, int[2], std::vector<std::vector<int>> &);
    bool Search(int, int, int, int);
    void Search2(int, int, int, int);
    void addObstaclesToVector();
    void addPathToVector();
    std::string CellString(State);

public:
    std::vector<std::vector<Level::State>> board;
    std::vector<SDL_Rect> obstacles;
    std::vector<SDL_Rect> path;

private:
    int n;
    const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
};