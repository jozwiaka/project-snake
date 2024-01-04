#include "renderer.h"
#include "level.h"
#include <iostream>
#include <string>

Renderer::Renderer(const Level &level) : level(level)
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, Params::kScreenWidth,
                                Params::kScreenHeight, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window)
  {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer)
  {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  block.w = Params::kCellWidth;
  block.h = Params::kCellHeight;
}

Renderer::~Renderer()
{
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake &snake, Enemy &enemy, SDL_Point const &food)
{
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
  // Render level
  if (enemy.createNewPath)
  {
    level.path.clear();
    level.updatePath(enemy.head_y, enemy.head_x, food.y, food.x);
    enemy.createNewPath = false;
    //level.PrintBoard();
    //std::cout<<"Created new Path";
  }
  for (int i = 0; i < level.board.size(); i++)
  {
    for (int j = 0; j < level.board[i].size(); j++)
    {
      if (level.board[i][j] == Level::State::kObstacle)
      {
        block.x = j * block.w;
        block.y = i * block.h;
        SDL_SetRenderDrawColor(sdl_renderer, 0x8B, 0x45, 0x13, 0xFF);
        SDL_RenderFillRect(sdl_renderer, &block);
      }
    }
  }
  // for (auto &p : level.path)
  // {
  //   SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xFF, 0x00, 0xFF);
  //   SDL_RenderFillRect(sdl_renderer, &p);
  // }
  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  //SNAKE:
  //snake body:
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body)
  {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  //snake head:
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive)
  {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  }
  else
  {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  //Check collision
  if (level.obsCollWithHead(block))
  {
    switch (snake.direction)
    {
    case Snake::Direction::kUp:
      snake.head_y += snake.speed;
      break;

    case Snake::Direction::kDown:
      snake.head_y -= snake.speed;
      break;

    case Snake::Direction::kLeft:
      snake.head_x += snake.speed;
      break;

    case Snake::Direction::kRight:
      snake.head_x -= snake.speed;
      break;
    }
    block.x = static_cast<int>(snake.head_x) * block.w;
    block.y = static_cast<int>(snake.head_y) * block.h;
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // ENEMY:
  // enemy body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : enemy.body)
  {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  enemy.direction = enemy.findRightDirection(level);
  block.x = static_cast<int>(enemy.head_x) * block.w;
  block.y = static_cast<int>(enemy.head_y) * block.h;
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  //enemy head
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int enemyScore, int fps)
{
  std::string title{"Snake Score: " + std::to_string(score) + " Enemy Score: " + std::to_string(enemyScore) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
