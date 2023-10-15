#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

#include "MyWindow.h"

size_t hash_combine(size_t lhs, size_t rhs);

struct pair_hash {
  std::size_t operator()(const std::pair<int, int>& pair) const {
    return hash_combine(std::hash<int>()(pair.first),
                        std::hash<int>()(pair.second));
  }
};

class Walls : public sf::Drawable {
 private:
  std::unordered_map<std::pair<int, int>, sf::RectangleShape, pair_hash>
      walls_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  void update(MyWindow& window);

  void emplace(const std::pair<std::pair<int, int>, sf::RectangleShape>& wall);
  sf::RectangleShape& operator[](const std::pair<int, int>& p);
  sf::RectangleShape& at(const std::pair<int, int>& p);
  size_t size() const;
};