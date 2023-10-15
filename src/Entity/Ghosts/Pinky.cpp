#include "Pinky.h"

const sf::Color Pinky::pinkyFillColor_m = sf::Color(255, 187, 255);

Pinky::Pinky(const sf::Texture& texture, int level)
    : Ghost(texture, pinkyStartMapX, pinkyStartMapY, pinkyStartPixX,
            pinkyStartPixY, pinkyStartTexRectL, pinkyStartTexRectT, level) {
  target_m.spr.setColor(pinkyFillColor_m);
}

sf::Color Pinky::getFillColor() { return pinkyFillColor_m; }

void Pinky::scatterChooseTarget(Pacman& pacman) {
  target_m.setPosition(pinkyScatterTargetMapX, pinkyScatterTargetMapY);
}

void Pinky::chaseChooseTarget(Pacman& pacman, Blinky& blinky) {
  Dir pacmanCurrDir = pacman.getCurrDir();
  Dir searchDir;
  if (pacmanCurrDir == Dir::Stop)
    searchDir = chasePrevDir_m;
  else {
    searchDir = chasePrevDir_m = pacmanCurrDir;
  }

  switch (searchDir) {
    case Up:
      target_m.setPosition(pacman.getCellCenterMapX(),
                           pacman.getCellCenterMapY() - targetOffset_m);
      break;
    case Left:
      target_m.setPosition(pacman.getCellCenterMapX() - targetOffset_m,
                           pacman.getCellCenterMapY());
      break;
    case Down:
      target_m.setPosition(pacman.getCellCenterMapX(),
                           pacman.getCellCenterMapY() + targetOffset_m);
      break;
    case Right:
      target_m.setPosition(pacman.getCellCenterMapX() + targetOffset_m,
                           pacman.getCellCenterMapY());
      break;
    case Stop:
      target_m.setPosition(pacman.getCellCenterMapX(),
                           pacman.getCellCenterMapY());
      break;
  }
}

void Pinky::reloadAfterPacmanDeath() {
  Ghost::reloadAfterPacmanDeath();
  chasePrevDir_m = Stop;
}