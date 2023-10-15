#pragma once

#include <string>
#include <vector>

#include "Globals.h"
#include "MyWindow.h"
#include "Walls.h"

class Map : public sf::Drawable {
  static const std::string imageFilename_m;

  std::vector<std::string> map_m;
  Walls walls_m;

  sf::Texture mapTex_m;
  sf::Sprite mapSpr_m;

  sf::Texture blackSquareTex_m;
  sf::Sprite blackSquareSpr_m;
  bool showCageEntrance_m = true;

  int rectLeft = 0;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void createWalls();

 public:
  Map();

  void update(MyWindow& window);
  void stopShowCageEntrance();
  void changeColor();
  void setDefault();

  void reloadNextLevel();
  void reloadNewGame();

  std::string& operator[](int y);
  const std::string& operator[](int y) const;

  char getCellState(int x, int y) const;
  const sf::RectangleShape& getWallRectangleShape(const std::pair<int, int>& p);
};