#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "level.h"
#include "enemy.h"
#include "params.h"

class Renderer {
 public:
  Renderer(const Level& level);
  ~Renderer();

  void Render(Snake& snake,Enemy& enemy, SDL_Point const &food);
  void UpdateWindowTitle(int score, int enemyScore, int fps);

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  SDL_Rect block;
  
  Level level;
};

#endif