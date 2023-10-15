#pragma once

#include <string>

#include "ClassicGameMusic.h"
#include "Globals.h"
#include "Map.h"
#include "MyWindow.h"
#include "Sprite2H.h"

class Entity : public sf::Drawable {
 public:
  enum Dir { Up = 0, Left, Down, Right, Stop };

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  static std::vector<std::pair<int, int>> wallsOffset_m;

 protected:
  float startSpriteCenterPixX_m, startSpriteCenterPixY_m;  // make const
  int cellCenterMapX_m, cellCenterMapY_m;
  float cellCenterPixX_m, cellCenterPixY_m;
  float spriteCenterPixX_m, spriteCenterPixY_m;
  int startTexRectL_m, startTexRectT_m;

  Sprite2H sprite2H_m;

  float dx_m = 0, dy_m = 0;
  int currFrame_m = 0;
  float accumTimeNextFrame_m = 0;
  Dir currDir_m = Stop, nextDir_m = Stop;
  float currSpeed_m = maxSpeed;
  bool hasTurned_m = false;

  Entity(const sf::Texture& texture, float hbWidth, int startPosMapX,
         int startPosMapY, float startPosPixX, float startPosPixY,
         int startTexRectL, int startTexRectT);
  Entity(const Entity& other) = default;
  Entity& operator=(const Entity& other) = default;
  ~Entity() override = default;

  bool wallCollision(Map& map) const;
  void updateVariables();
  void handlePortal(float moveDist);
  virtual void reloadAfterDeath();

 public:
  sf::FloatRect getHitboxBounds() const;
  sf::Vector2i getMapPosition() const;
  const sf::Texture* getTexture() const;
  float getSpriteCenterPixX() const;
  float getSpriteCenterPixY() const;
  int getCellCenterMapX() const;
  int getCellCenterMapY() const;
  const Dir& getCurrDir() const;
  const Dir& getNextDir() const;
  float getSpeed() const;
  static std::pair<int, int> pixelToMapCell(float x, float y);
};