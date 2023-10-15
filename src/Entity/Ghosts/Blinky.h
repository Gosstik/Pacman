#pragma once

#include "Ghost.h"

class Blinky : public Ghost {
 private:
  enum ElroyState { normal, elroy1, elroy2 };
  static constexpr int blinkyStartMapX = rightAboveCageCenterMapX;  // 5
  static constexpr int blinkyStartMapY = aboveCageCenterMapY;       // 1
  static constexpr float blinkyStartPixX =
      (blinkyStartMapX + mapOffsetCellX_i) *
      cellSize_i;  // aboveCageCenterPixX;
  static constexpr float blinkyStartPixY =
      (blinkyStartMapY + mapOffsetCellY_i + 0.5f) *
      cellSize_i;  // aboveCageCenterPixY;
  static constexpr int blinkyStartTexRectL =
      entitySize_i * ghostMaxFrameMove * Dir::Left;
  static constexpr int blinkyStartTexRectT = 0;
  static constexpr int blinkyScatterTargetMapX = 27;  // related to map
  static constexpr int blinkyScatterTargetMapY = -3;  // related to map
  static const sf::Color blinkyFillColor_m;

  const std::vector<int> elroyLvlChangeSpeedRatio_m{1, 2, 5};
  const std::vector<float> elroy1SpeedRatio_m{0.8f, 0.9f, 1.0f};
  static constexpr float elroy2SpeedRatio_m = 0.05f;

  const std::vector<int> elroyLvlChangeDotsLeft_m{1, 2, 3, 6, 9, 12, 15, 19};
  const std::vector<int> elroy1DotsLeftVec_m{20, 30, 40, 50, 60, 80, 100, 120};
  static constexpr int elroy2DotsLeftRatio_m = 2;

  float normalSpeedInNormalState_m;
  ElroyState elroyState_m;
  float elroy1Speed_m;
  float elroy2Speed_m;

  int elroy1DotsLeft_m;
  int elroy2DotsLeft_m;

  bool pacmanWasKilled_m = false;

  void updateElroyValues(int level);

 public:
  bool clydeLeftCage_m = false;

  explicit Blinky(const sf::Texture& texture, int level = 1);

  static sf::Color getFillColor();

  void handleElroyState(int allGumsLeft);

  void scatterChooseTarget(Pacman& pacman) override;
  void chaseChooseTarget(Pacman& pacman, Blinky& blinky) override;

  void reloadAfterPacmanDeath() override;
  void reloadNextLevel(int level, float normalSpeed, float tunnelSpeed,
                       float frightSpeed) override;
  void reloadNewGame(float normalSpeed, float tunnelSpeed,
                     float frightSpeed) override;
};