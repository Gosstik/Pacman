#include "Map.h"

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(mapSpr_m);
  if (!showCageEntrance_m) target.draw(blackSquareSpr_m);
  if (showEntityHitbox_b) target.draw(walls_m, states);
}

Map::Map() {
  map_m = {
      "   WWWWWWWWWWWW  WWWWWWWWWWWW   ", "  WSSSSSSSSSSSSWWSSSSSSSSSSSSW  ",
      "  WSWWWWSWWWWWSWWSWWWWWSWWWWSW  ", "  WEW  WSW   WSWWSW   WSW  WEW  ",
      "  WSWWWWSWWWWWSWWSWWWWWSWWWWSW  ", "  WSSSSSSSSSSSSSSSSSSSSSSSSSSW  ",
      "  WSWWWWSWWSWWWWWWWWSWWSWWWWSW  ", "  WSWWWWSWWSWWW  WWWSWWSWWWWSW  ",
      "  WSSSSSSWWSSSSWWSSSSWWSSSSSSW  ", "  WWWWWWSW WWW WW WWW WSWWWWW   ",
      "       WSW WWW WW WWW WSW       ", "       WSWW          WWSW       ",
      "       WSWW WWWWWWWW WWSW       ", "WWWWWWWWSWW W      W WWSWWWWWWWW",
      "        S   W      W   S        ", "WWWWWWWWSWW W      W WWSWWWWWWWW",
      "       WSWW WWWWWWWW WWSW       ", "       WSWW          WWSW       ",
      "       WSWW WWWWWWWW WWSW       ", "   WWWWWSWW WWW  WWW WWSWWWWW   ",
      "  WSSSSSSSSSSSSWWSSSSSSSSSSSSW  ", "  WSWWWWSWWWWWSWWSWWWWWSWWWWSW  ",
      "  WSWWWWSWWWWWSWWSWWWWWSWWWWSW  ", "  WESSWWSSSSSSS  SSSSSSSWWSSEW  ",
      "   WWSWWSWWSWWWWWWWWSWWSWWSWW   ", "   WWSWWSWWSWWW  WWWSWWSWWSWW   ",
      "  WSSSSSSWWSSSSWWSSSSWWSSSSSSW  ", "  WSWWWWWWWWWWSWWSWWWWWWWWWWSW  ",
      "  WSWWWWWWWWWWSWWSWWWWWWWWWWSW  ", "  WSSSSSSSSSSSSSSSSSSSSSSSSSSW  ",
      "   WWWWWWWWWWWWWWWWWWWWWWWWWW   ",
  };
  createWalls();

  mapTex_m.loadFromFile(imageFilename_m);
  mapSpr_m.setTexture(mapTex_m);
  mapSpr_m.setPosition(float(mapOffsetCellX_i) * cellSize_f,
                       float(mapOffsetCellY_i) * cellSize_f);
  setDefault();

  blackSquareTex_m.loadFromFile(blackSquare16ImageFilename_m);
  blackSquareSpr_m.setTexture(blackSquareTex_m);
  blackSquareSpr_m.setOrigin(blackSquareSpr_m.getGlobalBounds().width / 2.0f,
                             blackSquareSpr_m.getGlobalBounds().height / 2.0f);
  blackSquareSpr_m.setPosition(aboveCageCenterPixX,
                               aboveCageCenterPixY + cellSize_f);
  blackSquareSpr_m.setScale(2.0f, 2.0f);
}

void Map::update(MyWindow& window) {}

void Map::stopShowCageEntrance() { showCageEntrance_m = false; }

void Map::changeColor() {
  rectLeft += mapWidthPix;
  rectLeft %= 2 * mapWidthPix;
  mapSpr_m.setTextureRect({rectLeft, 0, mapWidthPix, mapHeightPix});
}

void Map::setDefault() {
  showCageEntrance_m = true;
  rectLeft = 0;
  mapSpr_m.setTextureRect({rectLeft, 0, mapWidthPix, mapHeightPix});
}

char Map::getCellState(int x, int y) const { return map_m[y][x]; }

void Map::createWalls() {
  for (int y = 0; y < mapHeightCell; ++y) {
    for (int x = 0; x < mapWidthCell; ++x) {
      if (map_m[y][x] == 'W' || map_m[y][x] == 'H') {
        std::pair<int, int> p{x, y};
        walls_m.emplace({{x, y}, sf::RectangleShape({cellSize_f, cellSize_f})});
        walls_m[p].setFillColor(sf::Color::Cyan);
        walls_m[p].setPosition(float(x + mapOffsetCellX_i) * cellSize_f,
                               float(y + mapOffsetCellY_i) * cellSize_f);
      }
    }
  }
}

const std::string& Map::operator[](int y) const { return map_m[y]; }

std::string& Map::operator[](int y) { return map_m[y]; }

void Map::reloadNextLevel() { setDefault(); }

void Map::reloadNewGame() { setDefault(); }

const sf::RectangleShape& Map::getWallRectangleShape(
    const std::pair<int, int>& p) {
  return walls_m[p];
}