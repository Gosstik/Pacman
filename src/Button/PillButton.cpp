#include "PillButton.h"

const sf::Color PillButton::deactivatedFillColor = sf::Color(60, 60, 60);
const sf::Color PillButton::deactivatedOutlineColor = sf::Color(255, 255, 255);
constexpr float PillButton::deactivatedOutlineThickness = 1.0f;
const sf::Color PillButton::activatedFillColor = sf::Color(0, 60, 255);
const sf::Color PillButton::activatedOutlineColor = sf::Color(0, 0, 255);
constexpr float PillButton::activatedOutlineThickness = 2.0f;

void PillButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(hitbox_m);
  target.draw(name_m);
}

PillButton::PillButton(Music& music, const std::string& name, float scale,
                       float x, float y, bool origInCenter,
                       const MyConvexShape::HitboxType& hbType)
    : Button(music, name, scale, x, y, origInCenter, hbType) {
  deactivate({0, 0});
}

void PillButton::update(sf::Event& event, sf::Vector2f& mousePos) {
  if (!isActivated_m) Button::update(event, mousePos);
}

void PillButton::activate() {
  Button::activate();
  isActivated_m = true;
  hitbox_m.setFillColor(activatedFillColor);
  hitbox_m.setOutlineThickness(activatedOutlineThickness);
  hitbox_m.setOutlineColor(activatedOutlineColor);
}

void PillButton::deactivate(const sf::Vector2f& mousePos) {
  if (hitbox_m.getGlobalBounds().contains(mousePos))
    setState(hover, false);
  else
    setState(idle, false);
  isActivated_m = false;
  hitbox_m.setFillColor(deactivatedFillColor);
  hitbox_m.setOutlineThickness(deactivatedOutlineThickness);
  hitbox_m.setOutlineColor(deactivatedOutlineColor);
}

void PillButton::reloadAfterOpenScene(sf::Vector2f& mousePos,
                                      bool wasReleased) {
  if (isActivated_m) {
    wasReleased_m = wasReleased;
    Button::activate();
  } else
    Button::reloadAfterOpenScene(mousePos, wasReleased);
}