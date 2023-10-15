#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Music.h"
#include "MyConvexShape.h"
#include "MyText.h"

class Button : public sf::Drawable {
 private:
  static constexpr float hitboxOutlineThickness_m = 1;
  static const sf::Color hitboxOutlineColor_m;
  static const sf::Color hitboxFillColor_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void updateScales();
  void updatePos();

 protected:
  enum ButtonState {
    idle,
    hover,
    pressed,
  };

  sf::Sound& hoverSoundRef_m;
  sf::Sound& pressSoundRef_m;
  sf::Sound& releaseSoundRef_m;

  ButtonState newState_m = idle;
  ButtonState currState_m = idle;

  sf::Color idleColor = sf::Color(180, 180, 180);
  sf::Color hoverColor = sf::Color::White;
  sf::Color pressedColor = sf::Color::White;
  sf::Color releasedColor = idleColor;

  float idleScale_m, hoverScale_m, pressedScale_m;
  float idlePosX_m, idlePosY_m;
  float hoverPosX_m, hoverPosY_m;
  float pressedPosX_m, pressedPosY_m;

  bool wasReleased_m = true;
  bool wasActivated_m = false;
  bool origInCenter_m;

  MyText name_m;
  MyConvexShape hitbox_m;

 public:
  Button(Music& music, const std::string& name, float scale = 1.0, float x = 0,
         float y = 0, bool origInCenter = true,
         const MyConvexShape::HitboxType& hbType =
             MyConvexShape::HitboxType::Rect);
  //    Button(const Button& other);
  //    Button& operator=(const Button& other);
  //    Button(Button&& other) noexcept;
  //    Button& operator=(Button&& other) noexcept;
  /// virtual
  virtual void update(sf::Event& event, sf::Vector2f& mousePos);
  virtual void activate();
  virtual void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased);

  void setPosition(float x, float y);
  void setScale(float scale);
  //    void setOrigin(float scaleX, float scaleY);
  void reload();
  void setState(const ButtonState& state, bool playSounds = true);
  bool wasActivated() const;
  //    sf::FloatRect getHitboxGlobalBounds() const;
  //    sf::FloatRect getHitboxLocalBounds() const;
};