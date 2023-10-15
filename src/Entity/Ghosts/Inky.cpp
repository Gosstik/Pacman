#include "Inky.h"

const sf::Color Inky::inkyFillColor_m = sf::Color(0, 255, 255);

Inky::Inky(const sf::Texture& texture, int level)
    : Ghost(texture, inkyStartMapX, inkyStartMapY, inkyStartPixX, inkyStartPixY,
            inkyStartTexRectL, inkyStartTexRectT, level) {
  target_m.spr.setColor(inkyFillColor_m);
}

sf::Color Inky::getFillColor() { return inkyFillColor_m; }

void Inky::scatterChooseTarget(Pacman& pacman) {
  target_m.setPosition(inkyScatterTargetMapX, inkyScatterTargetMapY);
}

void Inky::chaseChooseTarget(Pacman& pacman, Blinky& blinky) {
  int abovePacmanX, abovePacmanY;
  switch (pacman.getCurrDir()) {
    case Up:
      abovePacmanX = pacman.getCellCenterMapX();
      abovePacmanY = pacman.getCellCenterMapY() - targetOffset_m;
      break;
    case Left:
      abovePacmanX = pacman.getCellCenterMapX() - targetOffset_m;
      abovePacmanY = pacman.getCellCenterMapY();
      break;
    case Down:
      abovePacmanX = pacman.getCellCenterMapX();
      abovePacmanY = pacman.getCellCenterMapY() + targetOffset_m;
      break;
    case Right:
      abovePacmanX = pacman.getCellCenterMapX() + targetOffset_m;
      abovePacmanY = pacman.getCellCenterMapY();
      break;
    case Stop:
      abovePacmanX = pacman.getCellCenterMapX();
      abovePacmanY = pacman.getCellCenterMapY();
      break;
  }
  int blinkyMapX = blinky.getCellCenterMapX();
  int blinkyMapY = blinky.getCellCenterMapY();
  target_m.setPosition(blinkyMapX + 2 * (abovePacmanX - blinkyMapX),
                       blinkyMapY + 2 * (abovePacmanY - blinkyMapY));
}