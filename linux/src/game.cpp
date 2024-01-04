#include "game.h"
#include <iostream>

Game::Game(Level level)
    : snake(Params::kGridWidth * 1 / 4, Params::kGridHeight * 3 / 4),
      enemy(Params::kGridWidth * 3 / 4, Params::kGridHeight * 1 / 4),
      level(level),
      engine(dev()),
      random_w(0, static_cast<int>(Params::kGridWidth - 1)),
      random_h(0, static_cast<int>(Params::kGridHeight - 1))
{
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer)
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running)
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, this);
    Update();
    renderer.Render(snake, enemy, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000)
    {
      renderer.UpdateWindowTitle(score, enemyScore, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < Params::kMsPerFrame)
    {
      SDL_Delay(Params::kMsPerFrame - frame_duration);
    }
  }
}

void Game::PlaceFood()
{
  enemy.createNewPath = true;
  int x, y;
  SDL_Rect block;
  int i;
  while (true)
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    block.x = x * Params::kCellWidth;
    block.y = y * Params::kCellHeight;
    if (!snake.SnakeCell(x, y) && !enemy.SnakeCell(x, y))
    {
      i = 0;
      for (auto &o : level.obstacles)
      {
        if (o.x != block.x || o.y != block.y)
          i++;
      }
      if (i == level.obstacles.size())
      {
        food.x = x;
        food.y = y;
        return;
      }
    }
  }
}

void Game::Update()
{
  if (!snake.alive)
    return;

  snake.Update();
  enemy.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y)
  {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    //snake.speed += 0.02;
  }

  new_x = static_cast<int>(enemy.head_x);
  new_y = static_cast<int>(enemy.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y)
  {
    enemyScore++;
    PlaceFood();
    // Grow snake and increase speed.
    enemy.GrowBody();
    //snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetEnemyScore() const { return enemyScore; }
int Game::GetSize() const { return snake.size; }
int Game::GetEnemySize() const { return enemy.size; }