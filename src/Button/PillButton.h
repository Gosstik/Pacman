#pragma once

#include "Button.h"

class PillButton : public Button {
  static const sf::Color deactivatedFillColor;
  static const sf::Color deactivatedOutlineColor;
  static const float deactivatedOutlineThickness;
  static const sf::Color activatedFillColor;
  static const sf::Color activatedOutlineColor;
  static const float activatedOutlineThickness;

  bool isActivated_m = false;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  PillButton(Music& music, const std::string& name, float scale = 1.0,
             float x = 0, float y = 0, bool origInCenter = true,
             const MyConvexShape::HitboxType& hbType =
                 MyConvexShape::HitboxType::Pill);
  void deactivate(const sf::Vector2f& mousePos);

  /// virtual
  void update(sf::Event& event, sf::Vector2f& mousePos) override;
  void activate() override;
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased) override;
};