#include "Walls.h"

size_t hash_combine(size_t lhs, size_t rhs) {
  lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
  return lhs;
}

void Walls::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto& wall : walls_m) target.draw(wall.second);
}

void Walls::emplace(
    const std::pair<std::pair<int, int>, sf::RectangleShape>& wall) {
  walls_m.emplace(wall);
}

sf::RectangleShape& Walls::operator[](const std::pair<int, int>& p) {
  return walls_m[p];
}

sf::RectangleShape& Walls::at(const std::pair<int, int>& p) {
  return walls_m.at(p);
}

size_t Walls::size() const { return walls_m.size(); }

void Walls::update(MyWindow& window) {}