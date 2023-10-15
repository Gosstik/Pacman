#include "MyConvexShapeHelper.h"

sf::Vector2f getVectorAB(const sf::Vector2f& A, const sf::Vector2f& B) {
  return {B.x - A.x, B.y - A.y};
}

bool sinSign(const sf::Vector2f& a, const sf::Vector2f& b) {
  return a.x * b.y - a.y * b.x > 0;
}