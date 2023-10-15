#pragma once

#include "Globals.h"

class Sprite1H : public sf::Drawable {
 protected:
  sf::Sprite sprite_m;
  sf::RectangleShape hitbox_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  Sprite1H() = default;
  Sprite1H(const sf::Texture& texture, const sf::IntRect& rectangle,
           float hbWidth);
  Sprite1H(const Sprite1H& other);
  // Sprite1H& operator=(Sprite1H other);
  Sprite1H& operator=(const Sprite1H& other);
  Sprite1H(Sprite1H&& other) noexcept;
  Sprite1H& operator=(Sprite1H&& other) noexcept;
  void mySwap(Sprite1H& other);

  void create(const sf::Texture& texture, const sf::IntRect& rectangle,
              float width);
  void setTexture(const sf::Texture& texture);
  void setTextureRect(const sf::IntRect& rectangle);
  void setHitbox(float width);
  virtual void setOrigin(float x, float y);
  virtual void setPosition(float x, float y);
  virtual void move(float offsetX, float offsetY);
  void setRotation(float angle);

  sf::FloatRect getSpriteGlobalBounds() const;
  sf::FloatRect getSpriteLocalBounds() const;
  sf::FloatRect getHitboxGlobalBounds() const;
  sf::FloatRect getHitboxLocalBounds() const;
  const sf::Texture* getTexture() const;
  const sf::IntRect& getTextureRect() const;
  const sf::Vector2f& getPosition() const;
};