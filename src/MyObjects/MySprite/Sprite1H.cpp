#include "Sprite1H.h"

void Sprite1H::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_m);
  if (showEntityHitbox_b) target.draw(hitbox_m);
}

Sprite1H::Sprite1H(const sf::Texture& texture, const sf::IntRect& rectangle,
                   float hbWidth) {
  create(texture, rectangle, hbWidth);
}

Sprite1H::Sprite1H(const Sprite1H& other)
    : sprite_m(other.sprite_m), hitbox_m(other.hitbox_m) {
  sprite_m.setTexture(*other.sprite_m.getTexture());
}

//// copy-and-swap
// Sprite1H& Sprite1H::operator=(Sprite1H other) {
//     std::cout << "Sprite1H: copy assignment constructor\n";
//     if (this == &other)
//         return *this;
//     mySwap(other);
//     return *this;
// }

Sprite1H& Sprite1H::operator=(const Sprite1H& other) {
  if (this == &other) return *this;
  Sprite1H copy = other;  // calling copy constructor
  mySwap(copy);
  return *this;
}

Sprite1H::Sprite1H(Sprite1H&& other) noexcept {
  std::swap(sprite_m, other.sprite_m);
  std::swap(hitbox_m, other.hitbox_m);
}

Sprite1H& Sprite1H::operator=(Sprite1H&& other) noexcept {
  Sprite1H copy = std::move(other);  // calling moving constructor
  mySwap(copy);
  return *this;
}

void Sprite1H::mySwap(Sprite1H& other) {
  std::swap(sprite_m, other.sprite_m);
  std::swap(hitbox_m, other.hitbox_m);
}

void Sprite1H::create(const sf::Texture& texture, const sf::IntRect& rectangle,
                      float width) {
  sprite_m.setTexture(texture);
  sprite_m.setTextureRect(rectangle);
  setHitbox(width);
}

void Sprite1H::setTexture(const sf::Texture& texture) {
  sprite_m.setTexture(texture);
}

void Sprite1H::setTextureRect(const sf::IntRect& rectangle) {
  sprite_m.setTextureRect(rectangle);
}

void Sprite1H::setHitbox(float width) {
  if (width - 2 * hbThickness <= 0) {
    hitbox_m.setSize({width, width});
    hitbox_m.setFillColor(entityHbColor);
  } else {
    hitbox_m.setSize({width - 2 * hbThickness, width - 2 * hbThickness});
    hitbox_m.setOutlineThickness(hbThickness);
    hitbox_m.setOutlineColor(entityHbColor);
    hitbox_m.setFillColor(sf::Color::Transparent);
  }
}

void Sprite1H::setOrigin(float x, float y) {
  sprite_m.setOrigin(x, y);
  hitbox_m.setOrigin(
      hitbox_m.getSize().x * x / sprite_m.getLocalBounds().width,
      hitbox_m.getSize().y * y / sprite_m.getLocalBounds().height);
}

void Sprite1H::setPosition(float x, float y) {
  sprite_m.setPosition(x, y);
  hitbox_m.setPosition(x, y);
}

void Sprite1H::move(float offsetX, float offsetY) {
  sprite_m.move(offsetX, offsetY);
  hitbox_m.move(offsetX, offsetY);
}

void Sprite1H::setRotation(float angle) { sprite_m.setRotation(angle); }

sf::FloatRect Sprite1H::getSpriteGlobalBounds() const {
  return sprite_m.getGlobalBounds();
}

sf::FloatRect Sprite1H::getSpriteLocalBounds() const {
  return sprite_m.getLocalBounds();
}

sf::FloatRect Sprite1H::getHitboxGlobalBounds() const {
  return hitbox_m.getGlobalBounds();
}

sf::FloatRect Sprite1H::getHitboxLocalBounds() const {
  return hitbox_m.getLocalBounds();
}

const sf::Texture* Sprite1H::getTexture() const {
  return sprite_m.getTexture();
}

const sf::IntRect& Sprite1H::getTextureRect() const {
  return sprite_m.getTextureRect();
}

const sf::Vector2f& Sprite1H::getPosition() const {
  return sprite_m.getPosition();
}