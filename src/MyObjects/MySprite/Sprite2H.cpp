#include "Sprite2H.h"

void Sprite2H::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_m);
  if (showEntityHitbox_b) {
    target.draw(wallsHitbox_m);
    target.draw(hitbox_m);
  }
}

Sprite2H::Sprite2H(const sf::Texture& texture, const sf::IntRect& rectangle,
                   float hbWidth, float wallsHbWidth) {
  Sprite1H::create(texture, rectangle, hbWidth);
  setWallsHitbox(wallsHbWidth);
}

Sprite2H::Sprite2H(const Sprite2H& other)
    : Sprite1H(other), wallsHitbox_m(other.wallsHitbox_m) {}

Sprite2H& Sprite2H::operator=(const Sprite2H& other) {
  if (this == &other) return *this;
  Sprite2H copy = other;  // calling copy constructor
  mySwap(copy);
  return *this;
}

Sprite2H::Sprite2H(Sprite2H&& other) noexcept {
  std::swap(sprite_m, other.sprite_m);
  std::swap(hitbox_m, other.hitbox_m);

  std::swap(wallsHitbox_m, other.wallsHitbox_m);
}

Sprite2H& Sprite2H::operator=(Sprite2H&& other) noexcept {
  Sprite2H copy = std::move(other);  // calling moving constructor
  mySwap(copy);
  return *this;
}

void Sprite2H::mySwap(Sprite2H& other) {
  Sprite1H::mySwap(other);
  std::swap(wallsHitbox_m, other.wallsHitbox_m);
}

void Sprite2H::setOrigin(float x, float y) {
  Sprite1H::setOrigin(x, y);
  wallsHitbox_m.setOrigin(
      wallsHitbox_m.getSize().x * x / sprite_m.getLocalBounds().width,
      wallsHitbox_m.getSize().y * y / sprite_m.getLocalBounds().height);
}

void Sprite2H::setPosition(float x, float y) {
  Sprite1H::setPosition(x, y);
  wallsHitbox_m.setPosition(x, y);
}

void Sprite2H::move(float offsetX, float offsetY) {
  Sprite1H::move(offsetX, offsetY);
  wallsHitbox_m.move(offsetX, offsetY);
}

void Sprite2H::setWallsHitbox(float width) {
  if (width - 2 * hbThickness <= 0) {
    wallsHitbox_m.setSize({width, width});
    wallsHitbox_m.setFillColor(entityWallsHbColor);
  } else {
    wallsHitbox_m.setSize({width - 2 * hbThickness, width - 2 * hbThickness});
    wallsHitbox_m.setOutlineThickness(hbThickness);
    wallsHitbox_m.setOutlineColor(entityWallsHbColor);
    wallsHitbox_m.setFillColor(sf::Color::Transparent);
  }
}

sf::FloatRect Sprite2H::getWallsHitboxGlobalBounds() const {
  return wallsHitbox_m.getGlobalBounds();
}

sf::FloatRect Sprite2H::getWallsHitboxLocalBounds() const {
  return wallsHitbox_m.getLocalBounds();
}