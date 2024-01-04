#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "enemy.h"
#include "level.h"
#include "params.h"

class Game
{
public:
  Game(Level level);
  void Run(Controller const &controller, Renderer &renderer);
  int GetScore() const;
  int GetEnemyScore() const;
  int GetSize() const;
  int GetEnemySize() const;
  void PlaceFood();

private:
  Snake snake;
  Enemy enemy;
  Level level;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int enemyScore{0};

  void Update();
};

#endif