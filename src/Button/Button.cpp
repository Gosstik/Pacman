#include "Button.h"

const sf::Color Button::hitboxOutlineColor_m = sf::Color(255, 0, 0);
const sf::Color Button::hitboxFillColor_m = sf::Color(0, 0, 0, 0);

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (showButtonsHitbox_b) target.draw(hitbox_m);
  target.draw(name_m);
}

Button::Button(Music& music, const std::string& name, float scale, float x,
               float y, bool origInCenter,
               const MyConvexShape::HitboxType& hbType)
    : pressSoundRef_m(music.pressSound),
      hoverSoundRef_m(music.hoverSound),
      releaseSoundRef_m(music.releaseSound),
      idleScale_m(scale),
      idlePosX_m(x),
      idlePosY_m(y),
      origInCenter_m(origInCenter),
      name_m(name, idleScale_m, idleScale_m, idlePosX_m, idlePosY_m,
             origInCenter_m, idleColor),
      hitbox_m(hbType, name_m.getLocalBounds(), idleScale_m, idleScale_m,
               idlePosX_m, idlePosY_m, origInCenter_m) {
  hitbox_m.setOutlineThickness(hitboxOutlineThickness_m);
  hitbox_m.setOutlineColor(hitboxOutlineColor_m);
  hitbox_m.setFillColor(hitboxFillColor_m);
  updateScales();
  updatePos();
}

void Button::updateScales() {
  hoverScale_m = idleScale_m;
  pressedScale_m = idleScale_m * 0.9f;
}

void Button::updatePos() {
  pressedPosX_m = hoverPosX_m = idlePosX_m;
  pressedPosY_m = hoverPosY_m = idlePosY_m;
}

void Button::setPosition(float x, float y) {
  idlePosX_m = x;
  idlePosY_m = y;
  updatePos();
  name_m.setPosition(idlePosX_m, idlePosY_m);
  hitbox_m.setPosition(idlePosX_m, idlePosY_m);
}

void Button::setScale(float scale) {
  idleScale_m = scale;
  updateScales();
  name_m.setScale(idleScale_m, idleScale_m);
  hitbox_m.setScale(idleScale_m, idleScale_m);
}

void Button::update(sf::Event& event, sf::Vector2f& mousePos) {
  if (event.type == sf::Event::MouseButtonReleased &&
      event.mouseButton.button == sf::Mouse::Left) {
    wasReleased_m = true;
    if (hitbox_m.getGlobalBounds().contains(mousePos) &&
        newState_m == pressed) {
      if (pressSoundRef_m.getStatus() == sf::SoundSource::Stopped)
        releaseSoundRef_m.play();
      wasActivated_m = true;
      activate();
      return;
    }
  }
  if (wasReleased_m) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      wasReleased_m = false;
      if (hitbox_m.getGlobalBounds().contains(mousePos)) newState_m = pressed;
    } else if (hitbox_m.getGlobalBounds().contains(mousePos))
      newState_m = hover;
    else
      newState_m = idle;
  }

  if (newState_m != currState_m) {
    currState_m = newState_m;
    setState(newState_m);
  }
}

void Button::reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased) {
  wasReleased_m = wasReleased;
  if (wasReleased_m) {
    if (hitbox_m.getGlobalBounds().contains(mousePos))
      newState_m = hover;
    else
      newState_m = idle;
  }

  if (newState_m != currState_m) setState(newState_m);
}

void Button::reload() { wasActivated_m = false; }

void Button::setState(const ButtonState& state, bool playSounds) {
  currState_m = newState_m = state;
  switch (state) {
    case idle:
      name_m.setColor(idleColor);
      name_m.setScale(idleScale_m, idleScale_m);
      name_m.setPosition(idlePosX_m, idlePosY_m);
      break;
    case hover:
      if (playSounds) hoverSoundRef_m.play();
      name_m.setColor(hoverColor);
      name_m.setScale(hoverScale_m, hoverScale_m);
      name_m.setPosition(hoverPosX_m, hoverPosY_m);
      break;
    case pressed:
      if (playSounds) pressSoundRef_m.play();
      name_m.setColor(pressedColor);
      name_m.setScale(pressedScale_m, pressedScale_m);
      name_m.setPosition(pressedPosX_m, pressedPosY_m);
      break;
  }
}

void Button::activate() { setState(ButtonState::hover, false); }

bool Button::wasActivated() const { return wasActivated_m; }

// sf::FloatRect Button::getHitboxGlobalBounds() const {
//     return hitbox_m.getGlobalBounds();
// }
//
//
// sf::FloatRect Button::getHitboxLocalBounds() const {
//     return hitbox_m.getLocalBounds();
// }