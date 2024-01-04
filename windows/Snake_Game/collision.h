#pragma once
#include <SDL.h>
#undef main

class Collision
{
public:
    static bool check_collision(const SDL_Rect &, const SDL_Rect &);
};