#pragma once

#include <map>
#include <vector>

#include "Ghosts.h"
#include "Globals.h"
#include "Map.h"
#include "Pacman.h"
#include "Score.h"

class Gums : public sf::Drawable {
  static constexpr int rectLeft_m = 0;
  static constexpr int rectTop_m = 0;
  static constexpr int cost_m = 10;
  static constexpr float hbWidth_m = cellSize_f / 4;

  std::vector<std::vector<Sprite1H>> gums_m;
  Sprite1H mySprite_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  Gums(sf::Texture& texture, Map& map);
  void create(Map& map);
  void update(Pacman& pacman, Ghosts& ghosts, Score& score,
              ClassicGameMusic& CGMusic, bool& pacmanAteBonus);
  int count() const;

  static sf::IntRect getTextureRect();

  /// change in case adding or deleting variables
  void reloadAfterPacmanDeath(Map& map);
  void reloadNextLevel(Map& map);
  void reloadNewGame(Map& map);
};