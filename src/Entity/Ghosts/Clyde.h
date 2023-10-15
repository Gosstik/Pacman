#include "Ghost.h"

class Clyde : public Ghost {
 private:
  static constexpr int clydeStartMapX = cageRightCellMapX;  // 6
  static constexpr int clydeStartMapY = cageRightCellMapY;  // 1
  static constexpr float clydeStartPixX =
      (clydeStartMapX + mapOffsetCellX_i) * cellSize_i;  // cageRightCellPixX;
  static constexpr float clydeStartPixY =
      (clydeStartMapY + mapOffsetCellY_i + 0.5f) *
      cellSize_i;  // cageRightCellPixY;
  static constexpr int clydeStartTexRectL =
      entitySize_i * ghostMaxFrameMove * Dir::Down;
  static constexpr int clydeStartTexRectT = entitySize_i * 3;
  static constexpr int clydeScatterTargetMapX = 2;  // related to map
  static constexpr int clydeScatterTargetMapY =
      mapHeightCell;                                 // related to map
  static constexpr int squareOfTargetRadius_m = 64;  // in cells
  static const sf::Color clydeFillColor_m;

 public:
  explicit Clyde(const sf::Texture& texture, int level = 1);

  static sf::Color getFillColor();

  void scatterChooseTarget(Pacman& pacman) override;
  void chaseChooseTarget(Pacman& pacman, Blinky& blinky) override;
};