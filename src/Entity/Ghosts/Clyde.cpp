#include "Clyde.h"

const sf::Color Clyde::clydeFillColor_m = sf::Color(255, 187, 35);

Clyde::Clyde(const sf::Texture& texture, int level)
    : Ghost(texture, clydeStartMapX, clydeStartMapY, clydeStartPixX,
            clydeStartPixY, clydeStartTexRectL, clydeStartTexRectT, level) {
  target_m.spr.setColor(clydeFillColor_m);
}

sf::Color Clyde::getFillColor() { return clydeFillColor_m; }

void Clyde::scatterChooseTarget(Pacman& pacman) {
  target_m.setPosition(clydeScatterTargetMapX, clydeScatterTargetMapY);
}

void Clyde::chaseChooseTarget(Pacman& pacman, Blinky& blinky) {
  int pacmanMapX = pacman.getCellCenterMapX();
  int pacmanMapY = pacman.getCellCenterMapY();
  if (distToCell(pacmanMapX, pacmanMapY) > squareOfTargetRadius_m)
    target_m.setPosition(pacman.getCellCenterMapX(),
                         pacman.getCellCenterMapY());
  else
    target_m.setPosition(clydeScatterTargetMapX, clydeScatterTargetMapY);
}