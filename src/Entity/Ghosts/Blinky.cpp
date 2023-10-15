#include "Blinky.h"

const sf::Color Blinky::blinkyFillColor_m = sf::Color(255, 0, 0);

Blinky::Blinky(const sf::Texture& texture, int level)
    : Ghost(texture, blinkyStartMapX, blinkyStartMapY, blinkyStartPixX,
            blinkyStartPixY, blinkyStartTexRectL, blinkyStartTexRectT, level) {
  target_m.spr.setColor(blinkyFillColor_m);
  updateElroyValues(level);
  elroyState_m = ElroyState::normal;
  normalSpeedInNormalState_m = normalSpeed_m;
}

sf::Color Blinky::getFillColor() { return blinkyFillColor_m; }

void Blinky::updateElroyValues(int level) {
  size_t size = elroyLvlChangeSpeedRatio_m.size();
  for (size_t i = 0; i < size; ++i)
    if ((i == size - 1) || level < elroyLvlChangeSpeedRatio_m[i + 1]) {
      elroy1Speed_m = maxSpeed * elroy1SpeedRatio_m[i];
      elroy2Speed_m = elroy1Speed_m + maxSpeed * elroy2SpeedRatio_m;
      break;
    }

  size = elroyLvlChangeDotsLeft_m.size();
  for (size_t i = 0; i < size; ++i)
    if ((i == size - 1) || level < elroyLvlChangeDotsLeft_m[i + 1]) {
      elroy1DotsLeft_m = elroy1DotsLeftVec_m[i];
      elroy2DotsLeft_m = elroy1DotsLeft_m / elroy2DotsLeftRatio_m;
      break;
    }
}

void Blinky::scatterChooseTarget(Pacman& pacman) {
  if (elroyState_m == ElroyState::normal)
    target_m.setPosition(blinkyScatterTargetMapX, blinkyScatterTargetMapY);
  else
    target_m.setPosition(pacman.getCellCenterMapX(),
                         pacman.getCellCenterMapY());
}

void Blinky::chaseChooseTarget(Pacman& pacman, Blinky& blinky) {
  target_m.setPosition(pacman.getCellCenterMapX(), pacman.getCellCenterMapY());
}

void Blinky::handleElroyState(int allGumsLeft) {
  if (!pacmanWasKilled_m || clydeLeftCage_m) {
    if (elroyState_m != ElroyState::elroy2 && allGumsLeft <= elroy2DotsLeft_m) {
      elroyState_m = ElroyState::elroy2;
      normalSpeed_m = elroy2Speed_m;
    } else if (elroyState_m != ElroyState::elroy1 &&
               allGumsLeft <= elroy1DotsLeft_m) {
      elroyState_m = ElroyState::elroy1;
      normalSpeed_m = elroy1Speed_m;
    } else if (elroyState_m != ElroyState::normal &&
               allGumsLeft > elroy1DotsLeft_m) {
      elroyState_m = ElroyState::normal;
      normalSpeed_m = normalSpeedInNormalState_m;
    }
  } else if (elroyState_m != ElroyState::normal) {
    elroyState_m = ElroyState::normal;
    normalSpeed_m = normalSpeedInNormalState_m;
  }
}

void Blinky::reloadAfterPacmanDeath() {
  Ghost::reloadAfterPacmanDeath();
  elroyState_m = ElroyState::normal;
  normalSpeed_m = normalSpeedInNormalState_m;
  clydeLeftCage_m = false;
  pacmanWasKilled_m = true;
}

void Blinky::reloadNextLevel(int level, float normalSpeed, float tunnelSpeed,
                             float frightSpeed) {
  Ghost::reloadNextLevel(level, normalSpeed, tunnelSpeed, frightSpeed);
  elroyState_m = normal;
  normalSpeed_m = normalSpeedInNormalState_m = normalSpeed;
  updateElroyValues(level);
  clydeLeftCage_m = false;
  pacmanWasKilled_m = false;
}

void Blinky::reloadNewGame(float normalSpeed, float tunnelSpeed,
                           float frightSpeed) {
  Ghost::reloadNewGame(normalSpeed, tunnelSpeed, frightSpeed);
  elroyState_m = normal;
  normalSpeed_m = normalSpeedInNormalState_m = normalSpeed;
  updateElroyValues(1);
  clydeLeftCage_m = false;
  pacmanWasKilled_m = false;
}