#include "Ghost.h"

class Pinky : public Ghost {
 private:
  static constexpr int pinkyStartMapX = cageCenterMapX;  // 4
  static constexpr int pinkyStartMapY = cageCenterMapY;  // 1
  static constexpr float pinkyStartPixX =
      (pinkyStartMapX + mapOffsetCellX_i) * cellSize_i;  // cageCenterPixX;
  static constexpr float pinkyStartPixY =
      (pinkyStartMapY + mapOffsetCellY_i + 0.5f) *
      cellSize_i;  // cageCenterPixY;
  static constexpr int pinkyStartTexRectL =
      entitySize_i * ghostMaxFrameMove * Dir::Up;
  static constexpr int pinkyStartTexRectT = entitySize_i;
  static constexpr int pinkyScatterTargetMapX = 4;   // related to map
  static constexpr int pinkyScatterTargetMapY = -3;  // related to map
  static constexpr int targetOffset_m = 4;
  static const sf::Color pinkyFillColor_m;

  Dir chasePrevDir_m = Stop;

 public:
  explicit Pinky(const sf::Texture& texture, int level = 1);

  static sf::Color getFillColor();

  void scatterChooseTarget(Pacman& pacman) override;
  void chaseChooseTarget(Pacman& pacman, Blinky& blinky) override;

  void reloadAfterPacmanDeath() override;
};