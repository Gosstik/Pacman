#pragma once

#include "Globals.h"
#include "MyConvexShapeHelper.h"

class MyConvexShape : public sf::ConvexShape {
 public:
  enum HitboxType { Rect, Pill };

 private:
  static constexpr float rectHBOffset_m = cellSize_f / 8.0f;  // == cellSize / 4
  static constexpr float ovalHBOffset_m = cellSize_f / 4.0f;
  static constexpr size_t ovalPointCount_m = 32;

  HitboxType hbType_m = HitboxType::Rect;

 public:
  MyConvexShape() = default;
  MyConvexShape(const HitboxType& hbType, const sf::FloatRect& obj,
                float scaleX, float scaleY, float posX, float posY,
                bool origInCenter = true);
  bool contains(const sf::Vector2f& p);
};