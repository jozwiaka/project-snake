//#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "level.h"
#include "fstream"
#include <ctime>
#include "params.h"


int main() {
  
  
  Level level(1);
  Renderer renderer(level);
  Controller controller;
  Game game(level);
  game.Run(controller, renderer);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}