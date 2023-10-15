#include "SettingHeaderButton.h"

SettingHeaderButton::SettingHeaderButton(
    Music& music, const std::string& name, float scale, float x, float y,
    bool origInCenter, const MyConvexShape::HitboxType& hbType)
    : Button(music, name, scale, x, y, origInCenter, hbType) {}

void SettingHeaderButton::update(sf::Event& event, sf::Vector2f& mousePos) {
  if (!isActivated_m) Button::update(event, mousePos);
}

void SettingHeaderButton::activate() {
  Button::activate();
  isActivated_m = true;
}

void SettingHeaderButton::deactivate() {
  setState(ButtonState::idle, false);
  isActivated_m = false;
}

void SettingHeaderButton::reloadAfterOpenScene(sf::Vector2f& mousePos,
                                               bool wasReleased) {
  isActivated_m = false;
  Button::reloadAfterOpenScene(mousePos, wasReleased);
}

bool SettingHeaderButton::isActivated() const { return isActivated_m; }