#pragma once

#include <random>

#include "Globals.h"
#include "Gums.h"

class Fruits : public sf::Drawable {
  struct Fruit {
    int cost;
    int minLevel;
    int rectLeft;
    int rectTop;
  };

  static constexpr int firstFruitRectLeft_m = 0;
  static constexpr int firstFruitRectTop_m = 0;
  static constexpr float hbWidth_m = entityHbWidth;

  static constexpr int firstBorder_m = 170;
  static constexpr int secondBorder_m = 70;
  static constexpr int fruitPosMapX_m = 16;
  static constexpr int fruitPosMapY_m = 23;
  static constexpr float fruitPosPixX_m =
      (fruitPosMapX_m + mapOffsetCellX_i) * cellSize_i;
  static constexpr float fruitPosPixY_m =
      (fruitPosMapY_m + mapOffsetCellY_i + 0.5f) * cellSize_i;
  static constexpr float minTimeFruitExists_m = 9 * mcsInSec;
  static constexpr int fruitScoreRectHeight_m = 40;
  static constexpr float constTimeFruitScoreExists_m = 2 * mcsInSec;

  bool firstAppeared_m = false;
  bool secondAppeared_m = false;
  bool fruitExists_m = false;
  bool fruitScoreExists_m = false;
  int curr_m = 0;
  std::vector<Fruit> fruits_m;
  Sprite1H fruitsMySpr_m;
  sf::Sprite fruitScoreSpr_m;

  // times
  float accumTimeFruitExists_m = 0;
  float constTimeFruitExists_m = 0;
  float accumTimeFruitScoreExists_m = 0;

  // additional
  std::random_device seed_m;
  std::mt19937 gen_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  explicit Fruits(sf::Texture& fruitsTex, sf::Texture& fruitsScoreTex,
                  int level);
  void update(Pacman& pacman, Ghosts& ghosts, Score& score, Gums& gums,
              ClassicGameMusic& CGMusic, bool& pacmanAteBonus, float dTime);
  const sf::IntRect& getTextureRect() const;

  /// change in case adding or deleting variables
  void reloadAfterPacmanDeath(Map& map);
  void reloadNextLevel(int level);
  void reloadNewGame();
};