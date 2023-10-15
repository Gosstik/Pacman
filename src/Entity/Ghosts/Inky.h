#include "Blinky.h"

class Inky : public Ghost {
 private:
  static constexpr int inkyStartMapX = cageLeftCellMapX;  // 5
  static constexpr int inkyStartMapY = cageLeftCellMapY;  // 1
  static constexpr float inkyStartPixX =
      (inkyStartMapX + mapOffsetCellX_i) * cellSize_i;  // cageLeftCellPixX;
  static constexpr float inkyStartPixY =
      (inkyStartMapY + mapOffsetCellY_i + 0.5f) *
      cellSize_i;  // cageLeftCellPixY;
  static constexpr int inkyStartTexRectL =
      entitySize_i * ghostMaxFrameMove * Dir::Down;
  static constexpr int inkyStartTexRectT = entitySize_i * 2;
  static constexpr int inkyScatterTargetMapX = 29;             // related to map
  static constexpr int inkyScatterTargetMapY = mapHeightCell;  // related to map
  static constexpr int targetOffset_m = 2;
  static const sf::Color inkyFillColor_m;

 public:
  explicit Inky(const sf::Texture& texture, int level = 1);

  static sf::Color getFillColor();

  void scatterChooseTarget(Pacman& pacman) override;
  void chaseChooseTarget(Pacman& pacman, Blinky& blinky) override;
};