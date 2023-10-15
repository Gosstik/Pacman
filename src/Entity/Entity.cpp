#include "Entity.h"

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite2H_m);
}

std::vector<std::pair<int, int>> Entity::wallsOffset_m;

Entity::Entity(const sf::Texture& texture, float hbWidth, int startPosMapX,
               int startPosMapY, float startPosPixX, float startPosPixY,
               int startTexRectL, int startTexRectT)
    : startSpriteCenterPixX_m(startPosPixX),
      startSpriteCenterPixY_m(startPosPixY),
      cellCenterMapX_m(startPosMapX),
      cellCenterMapY_m(startPosMapY),
      cellCenterPixX_m(startPosPixX),
      cellCenterPixY_m(startPosPixY),
      spriteCenterPixX_m(startPosPixX),
      spriteCenterPixY_m(startPosPixY),
      startTexRectL_m(startTexRectL),
      startTexRectT_m(startTexRectT),
      sprite2H_m(texture,
                 {startTexRectL_m, startTexRectT_m, entitySize_i, entitySize_i},
                 hbWidth, entityWallsHbWidth) {
  sprite2H_m.setOrigin(sprite2H_m.getSpriteLocalBounds().width / 2.0f,
                       sprite2H_m.getSpriteLocalBounds().height / 2.0f);
  sprite2H_m.setPosition(spriteCenterPixX_m, spriteCenterPixY_m);
  if (wallsOffset_m.empty()) {
    wallsOffset_m.resize(4);
    wallsOffset_m[0] = {0, 0};
    wallsOffset_m[1] = {1, 0};
    wallsOffset_m[2] = {0, 1};
    wallsOffset_m[3] = {1, 1};
  }
}

bool Entity::wallCollision(Map& map) const {
  sf::Rect<float> sprHbGb = sprite2H_m.getWallsHitboxGlobalBounds();
  std::pair<int, int> lt = pixelToMapCell(sprHbGb.left, sprHbGb.top);
  for (auto& wo : wallsOffset_m) {
    if (map.getCellState(lt.first + wo.first, lt.second + wo.second) == 'W' &&
        map.getWallRectangleShape({lt.first + wo.first, lt.second + wo.second})
            .getGlobalBounds()
            .intersects(sprHbGb)) {
      return true;
    }
  }
  return false;
}

void Entity::updateVariables() {
  cellCenterMapX_m = int(spriteCenterPixX_m / cellSize_f) - mapOffsetCellX_i;
  cellCenterMapY_m = int(spriteCenterPixY_m / cellSize_f) - mapOffsetCellY_i;
  cellCenterPixX_m =
      (float(cellCenterMapX_m + mapOffsetCellX_i) + 0.5f) * cellSize_f;
  cellCenterPixY_m =
      (float(cellCenterMapY_m + mapOffsetCellY_i) + 0.5f) * cellSize_f;
}

sf::FloatRect Entity::getHitboxBounds() const {
  return sprite2H_m.getHitboxGlobalBounds();
}

sf::Vector2i Entity::getMapPosition() const {
  return {cellCenterMapX_m, cellCenterMapY_m};
}

const sf::Texture* Entity::getTexture() const {
  return sprite2H_m.getTexture();
}

float Entity::getSpriteCenterPixX() const { return spriteCenterPixX_m; }

float Entity::getSpriteCenterPixY() const { return spriteCenterPixY_m; }

int Entity::getCellCenterMapX() const { return cellCenterMapX_m; }

int Entity::getCellCenterMapY() const { return cellCenterMapY_m; }

const Entity::Dir& Entity::getCurrDir() const { return currDir_m; }

const Entity::Dir& Entity::getNextDir() const { return nextDir_m; }

float Entity::getSpeed() const { return currSpeed_m; }

std::pair<int, int> Entity::pixelToMapCell(float x, float y) {
  return {int(x / cellSize_f) - mapOffsetCellX_i,
          int(y / cellSize_f) - mapOffsetCellY_i};
}

void Entity::handlePortal(float moveDist) {
  if (currDir_m == Left && spriteCenterPixY_m == leftPortalCenterPixY &&
      spriteCenterPixX_m - leftPortalCenterPixX < moveDist) {
    moveDist -= spriteCenterPixX_m - leftPortalCenterPixX;
    sprite2H_m.setPosition(rightPortalCenterPixX - moveDist,
                           rightPortalCenterPixY);
    spriteCenterPixX_m = rightPortalCenterPixX - moveDist;
    spriteCenterPixY_m = rightPortalCenterPixY;
    updateVariables();
    return;
  } else if (currDir_m == Right &&
             spriteCenterPixY_m == rightPortalCenterPixY &&
             rightPortalCenterPixX - spriteCenterPixX_m < moveDist) {
    moveDist -= rightPortalCenterPixX - spriteCenterPixX_m;
    if (moveDist <= 0) {
      std::cerr << "Entity::handlePortal : moveDist <= 0\n";
      moveDist = 0;
    }
    sprite2H_m.setPosition(leftPortalCenterPixX + moveDist,
                           leftPortalCenterPixY);
    spriteCenterPixX_m = leftPortalCenterPixX + moveDist;
    spriteCenterPixY_m = leftPortalCenterPixY;
    updateVariables();
    return;
  }
}

void Entity::reloadAfterDeath() {
  spriteCenterPixX_m = startSpriteCenterPixX_m;
  spriteCenterPixY_m = startSpriteCenterPixY_m;
  updateVariables();
  sprite2H_m.setPosition(spriteCenterPixX_m, spriteCenterPixY_m);
  sprite2H_m.setTextureRect(
      {startTexRectL_m, startTexRectT_m, entitySize_i, entitySize_i});
  sprite2H_m.setRotation(0);  // for pacman

  dx_m = dy_m = 0;
  currFrame_m = 0;
  accumTimeNextFrame_m = 0;
  currDir_m = nextDir_m = Stop;
  hasTurned_m = true;
}