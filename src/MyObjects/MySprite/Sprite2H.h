#pragma once

#include "Sprite1H.h"

class Sprite2H : public Sprite1H {
 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 protected:
  sf::RectangleShape wallsHitbox_m;

 public:
  Sprite2H() = default;
  Sprite2H(const sf::Texture& texture, const sf::IntRect& rectangle1,
           float hbWidth, float wallsHbWidth);
  Sprite2H(const Sprite2H& other);
  Sprite2H& operator=(const Sprite2H& other);
  Sprite2H(Sprite2H&& other) noexcept;
  Sprite2H& operator=(Sprite2H&& other) noexcept;
  void mySwap(Sprite2H& other);

  /// virtual functions
  void setOrigin(float x, float y) override;
  void setPosition(float x, float y) override;
  void move(float offsetX, float offsetY) override;

  void setWallsHitbox(float width);
  sf::FloatRect getWallsHitboxGlobalBounds() const;
  sf::FloatRect getWallsHitboxLocalBounds() const;
};