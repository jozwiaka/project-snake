#pragma once
#include <vector>
#include "SDL.h"
#include "snake.h"
#include "level.h"
#include <cmath>
#include "params.h"
#include <iterator>

class Enemy : public Snake
{
public:
    Enemy(float head_x, float head_y) : Snake(head_x, head_y)
    {
        Snake::speed = 0.1f; //Enemy has his own speed
    }
    Direction findRightDirection(Level &);
    bool createNewPath;
};
