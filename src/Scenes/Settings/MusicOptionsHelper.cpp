#include "MusicOptionsHelper.h"

const sf::Color Volume::outlineColor_m = sf::Color(255, 255, 255);
const sf::Color Volume::lineFillColor_m = sf::Color(50, 50, 50);
const sf::Color Volume::sliderIdleFillColor_m = sf::Color(80, 80, 80);
const sf::Color Volume::sliderActiveFillColor_m = sf::Color(40, 40, 40);

Volume::Volume(Music& music, const std::string& name, float scale,
               float namePosX, float namePosY, float lineRightPosX)
    : musicRef_m(music),
      hoverSoundRef_m(music.hoverSound),
      pressSoundRef_m(music.pressSound),
      releaseSoundRef_m(music.releaseSound),
      linePosY_m(namePosY + optionsScale * cellSize_f / 2.0f),
      lineRightBorder_m(lineRightPosX),
      name_m(name, optionsScale, optionsScale, namePosX, namePosY, false),
      line_m({lineWidth_m, lineHeight_m}),
      slider_m({sliderWidth_m, sliderHeight_m}) {
  line_m.setOutlineThickness(lineOutlineThickness_m);
  line_m.setFillColor(lineFillColor_m);
  line_m.setOutlineColor(outlineColor_m);
  line_m.setOrigin(line_m.getLocalBounds().width / 2.0f,
                   line_m.getLocalBounds().height / 2.0f);
  line_m.setScale(scale, scale);
  line_m.setPosition(lineRightBorder_m - line_m.getGlobalBounds().width / 2.0f,
                     linePosY_m);

  lineLeftBorder_m = lineRightPosX - line_m.getGlobalBounds().width;

  slider_m.setOutlineThickness(sliderIdleOutlineThickness_m);
  slider_m.setFillColor(sliderIdleFillColor_m);
  slider_m.setOutlineColor(outlineColor_m);
  slider_m.setOrigin(slider_m.getLocalBounds().width / 2.0f,
                     slider_m.getLocalBounds().height / 2.0f);
  slider_m.setScale(scale, scale);
}

void Volume::handleEvents(sf::Event& event, sf::Vector2f& mousePos) {
  if (currState_m == SliderState::active) {
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
      wasReleased_m = true;
      if (slider_m.getGlobalBounds().contains(mousePos))
        setState(SliderState::hover, false);
      else
        setState(SliderState::idle);
      musicRef_m.setVolume(
          soundKind_m, 100.0f * (slider_m.getPosition().x - lineLeftBorder_m) /
                           line_m.getGlobalBounds().width);
      releaseSoundRef_m.play();
      return;
    }

    if (mousePos.x - mousePosOffsetX_m > lineRightBorder_m)
      slider_m.setPosition(lineRightBorder_m, linePosY_m);
    else if (mousePos.x - mousePosOffsetX_m < lineLeftBorder_m)
      slider_m.setPosition(lineLeftBorder_m, linePosY_m);
    else
      slider_m.setPosition(mousePos.x - mousePosOffsetX_m, linePosY_m);
    musicRef_m.setVolume(
        soundKind_m, 100.0f * (slider_m.getPosition().x - lineLeftBorder_m) /
                         line_m.getGlobalBounds().width);
  } else {
    if (wasReleased_m) {
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        wasReleased_m = false;
        if (slider_m.getGlobalBounds().contains(mousePos)) {
          newState_m = active;
          mousePosOffsetX_m = mousePos.x - (slider_m.getGlobalBounds().left +
                                            slider_m.getOrigin().x);
          mousePosOffsetY_m = mousePos.y - (slider_m.getGlobalBounds().top +
                                            slider_m.getOrigin().y);
        } else
          newState_m = idle;
      } else if (slider_m.getGlobalBounds().contains(mousePos))
        newState_m = hover;
      else
        newState_m = idle;
    } else if (event.type == sf::Event::MouseButtonReleased &&
               event.mouseButton.button == sf::Mouse::Left) {
      wasReleased_m = true;
      if (slider_m.getGlobalBounds().contains(mousePos))
        newState_m = hover;
      else
        newState_m = idle;
    }
  }

  if (newState_m != currState_m) setState(newState_m);
}

void Volume::reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased) {
  wasReleased_m = wasReleased;
  if (wasReleased_m) {
    if (slider_m.getGlobalBounds().contains(mousePos))
      newState_m = hover;
    else
      newState_m = idle;
  }

  if (newState_m != currState_m) setState(newState_m);
}

void Volume::setState(const SliderState& state, bool playSounds) {
  currState_m = newState_m = state;
  switch (state) {
    case idle:
      slider_m.setFillColor(sliderIdleFillColor_m);
      slider_m.setOutlineThickness(sliderIdleOutlineThickness_m);
      break;
    case hover:
      if (playSounds) hoverSoundRef_m.play();
      slider_m.setFillColor(sliderIdleFillColor_m);
      slider_m.setOutlineThickness(sliderHoverOutlineThickness_m);
      break;
    case active:
      if (playSounds) pressSoundRef_m.play();
      slider_m.setFillColor(sliderActiveFillColor_m);
      slider_m.setOutlineThickness(sliderIdleOutlineThickness_m);
      break;
  }
}