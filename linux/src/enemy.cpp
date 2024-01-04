#include "enemy.h"
#include "SDL.h"

Enemy::Direction Enemy::findRightDirection(Level &level)
{
    SDL_Rect bHead;
    bHead.x = static_cast<int>(Snake::head_x) * Params::kCellWidth;
    bHead.y = static_cast<int>(Snake::head_y) * Params::kCellHeight;
    SDL_Rect bNext;
    bNext.x = __INT_MAX__;
    bNext.y = __INT_MAX__;
    double distanceBetweenHeadAndP;
    double distanceBetweenHeadAndNext;
    std::vector<SDL_Rect>::iterator itToErase;
    if (level.path.empty())
        return Direction::kIdle;

    for (auto it = level.path.begin(); it != level.path.end(); it++)
    {
        distanceBetweenHeadAndP = pow(bHead.x - (*it).x, 2) + pow(bHead.y - (*it).y, 2);
        distanceBetweenHeadAndNext = pow(bHead.x - bNext.x, 2) + pow(bHead.y - bNext.y, 2);
        if (distanceBetweenHeadAndP < distanceBetweenHeadAndNext)
        {
            bNext = *it;
            itToErase = it;
        }
    }
    if (bHead.x == bNext.x && bHead.y == bNext.y)
        level.path.erase(itToErase);
    if (bNext.x - bHead.x < 0)
        return Direction::kLeft;
    if (bNext.x - bHead.x > 0)
        return Direction::kRight;
    if (bNext.y - bHead.y > 0)
        return Direction::kDown;
    if (bNext.y - bHead.y < 0)
        return Direction::kUp;

    return Direction::kIdle;
}