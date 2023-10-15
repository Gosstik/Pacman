#include "MyConvexShape.h"

MyConvexShape::MyConvexShape(const HitboxType& hbType, const sf::FloatRect& obj,
                             float scaleX, float scaleY, float posX, float posY,
                             bool origInCenter) {
  hbType_m = hbType;
  if (hbType == HitboxType::Rect) {
    float width = obj.width + rectHBOffset_m * 2;
    float height = obj.height + rectHBOffset_m * 2;
    setPointCount(4);
    // go clockwise
    setPoint(0, {0, 0});
    setPoint(1, {width, 0});
    setPoint(2, {width, height});
    setPoint(3, {0, height});
    if (origInCenter)
      setOrigin(width / 2.0f, height / 2.0f);
    else
      setOrigin({rectHBOffset_m, rectHBOffset_m});
  } else {
    setPointCount(ovalPointCount_m);
    float radius = obj.height / 2.0f + ovalHBOffset_m;
    float leftCenterX = radius;
    float rightCenterX = radius + obj.width;
    float centerY = radius;
    float dAlpha = 2.0f * pi / float(ovalPointCount_m - 2);
    float currAlpha = 0;
    size_t i = 0;
    for (; i < ovalPointCount_m / 2; ++i) {
      setPoint(i, {rightCenterX + radius * std::sin(currAlpha),
                   centerY + radius * std::cos(currAlpha)});
      currAlpha += dAlpha;
    }
    currAlpha -= dAlpha;
    for (; i < ovalPointCount_m; ++i) {
      setPoint(i, {leftCenterX + radius * std::sin(currAlpha),
                   centerY + radius * std::cos(currAlpha)});
      currAlpha += dAlpha;
    }
    if (origInCenter)
      setOrigin({radius + obj.width / 2.0f, radius});
    else
      setOrigin({radius, ovalHBOffset_m});
  }
  setScale({scaleX, scaleY});
  setPosition(posX, posY);
}

// if angle is from 0 to pi (because polygon is defined counterclockwise, else
// angle must be from -pi to 0) then the point is inside the convex polygon.
// In our case polygon is defined clockwise.
bool MyConvexShape::contains(const sf::Vector2f& p) {
  if (hbType_m == HitboxType::Rect)
    return getGlobalBounds().contains(p);
  else {
    sf::Vector2f firVec, secVec;
    firVec = getVectorAB(p, getPoint(ovalPointCount_m - 1));
    secVec = getVectorAB(p, getPoint(0));
    if (sinSign(firVec, secVec)) return false;
    for (size_t i = 1; i < ovalPointCount_m; ++i) {
      firVec = secVec;
      secVec = getVectorAB(p, getPoint(i));
      if (sinSign(firVec, secVec)) return false;
    }
    return true;
  }
}