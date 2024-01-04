#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include "params.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight, kIdle};

  Snake(float head_x, float head_y) :
        head_x(head_x),
        head_y(head_y) {}


  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kIdle;
  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
};

#endif