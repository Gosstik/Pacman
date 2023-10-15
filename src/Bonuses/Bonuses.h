#pragma once

#include "BigGums.h"
#include "ClassicGameMusic.h"
#include "Fruits.h"
#include "Gums.h"

class Bonuses : public sf::Drawable {
  static const std::string fruitsFilename_m;
  static const std::string gumsFilename_m;
  static const std::string fruitScoreFilename_m;

  sf::Image fruitsImg_m;
  sf::Texture fruitsTex_m;

  sf::Image gumsImg_m;
  sf::Texture gumsTex_m;

  sf::Image fruitScoreImg_m;
  sf::Texture fruitScoreTex_m;

  Gums gums_m;
  BigGums bigGums_m;
  Fruits fruits_m;

  float accumPacmanCoveredDistOneIter_m = 0;
  bool pacmanAteBonus_m = false;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void handlePacmanMunchAndSpeed(Pacman& pacman, Ghosts& ghosts,
                                 ClassicGameMusic& CGMusic, bool frightMode);

 public:
  Bonuses(Map& map, Pacman& pacman, int level);
  void update(Map& map, Pacman& pacman, Ghosts& ghosts, Score& score,
              ClassicGameMusic& CGMusic, bool frightMode, float dTime);
  void showBigGums();

  const sf::Texture* getGumsTexture() const;
  const sf::Texture* getFruitsTexture() const;
  const sf::IntRect& getFruitTextureRect() const;
  int gumsLeft() const;
  int bigGumsLeft() const;
  int allGumsLeft() const;

  /// change in case adding or deleting variables
  void reloadAfterPacmanDeath(Map& map);
  void reloadNextLevel(Map& map, int level);
  void reloadNewGame(Map& map);
};