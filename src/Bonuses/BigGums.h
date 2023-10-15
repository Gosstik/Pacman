#pragma once

#include <map>
#include <vector>

#include "Ghosts.h"
#include "Globals.h"
#include "Map.h"
#include "Pacman.h"
#include "Score.h"

class BigGums : public sf::Drawable {
  static constexpr int rectLeft_m = cellSize_i + cellSize_i / 8;
  static constexpr int rectTop_m = 0;
  static constexpr int cost_m = 50;
  static constexpr float hbWidth_m = cellSize_f / 2.0f;

  static constexpr float bigGumLeftTopPixX_m =
      (float(mapOffsetCellX_i) + 3.5f) * cellSize_f;
  static constexpr float bigGumLeftTopPixY_m =
      (float(mapOffsetCellY_i) + 3.5f) * cellSize_f;
  static constexpr float bigGumRightTopPixX_m =
      (float(mapOffsetCellX_i) + 28.5f) * cellSize_f;
  static constexpr float bigGumRightTopPixY_m = bigGumLeftTopPixY_m;
  static constexpr float bigGumLeftBottomPixX_m = bigGumLeftTopPixX_m;
  static constexpr float bigGumLeftBottomPixY_m =
      (float(mapOffsetCellY_i) + 23.5f) * cellSize_f;
  static constexpr float bigGumRightBottomPixX_m = bigGumRightTopPixX_m;
  static constexpr float bigGumRightBottomPixY_m = bigGumLeftBottomPixY_m;
  static constexpr float constTimeShowBigGums_m = 0.2 * mcsInSec;

  std::vector<Sprite1H> bigGums_m;
  Sprite1H mySprite_m;

  float accumTimeShowBigGums_m = 0;
  bool showBigGums_m = true;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  void create();

 public:
  explicit BigGums(sf::Texture& texture);
  void update(Map& map, Pacman& pacman, Ghosts& ghosts, Score& score,
              ClassicGameMusic& CGMusic, bool& pacmanAteBonus, float dTime);
  void show();
  int count() const;

  static sf::IntRect getTextureRect();

  /// change in case adding or deleting variables
  void reloadAfterPacmanDeath();
  void reloadNextLevel();
  void reloadNewGame();
};