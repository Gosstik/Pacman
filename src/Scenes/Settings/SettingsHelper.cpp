#include "SettingsHelper.h"

void BoolOption::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(name_m);
  if (currState_m)
    target.draw(yes_m);
  else
    target.draw(no_m);
  target.draw(leftArrow_m);
  target.draw(rightArrow_m);
}

BoolOption::BoolOption(Music& music, const std::string& name, float namePosY,
                       bool& currState)
    : currState_m(currState),
      statePosY_m(namePosY + cellSize_f * optionsScale / 2.0f),
      leftArrow_m(music, "<", arrowsScale, leftArrowPosX_m, statePosY_m, true),
      rightArrow_m(music, ">", arrowsScale, rightArrowPosX, statePosY_m, true),
      yes_m("yes", optionsScale, optionsScale, centerPosX_m, statePosY_m, true),
      no_m("no", optionsScale, optionsScale, centerPosX_m, statePosY_m, true),
      name_m(name, optionsScale, optionsScale, MMOptionNamePosX, namePosY,
             false) {}

void BoolOption::reloadAfterOpenScene(sf::Vector2f& mousePos,
                                      bool wasReleased) {
  leftArrow_m.reloadAfterOpenScene(mousePos, wasReleased);
  rightArrow_m.reloadAfterOpenScene(mousePos, wasReleased);
}

void BoolOption::handleEvents(sf::Event& event, sf::Vector2f& mousePos) {
  leftArrow_m.update(event, mousePos);
  rightArrow_m.update(event, mousePos);
  if (leftArrow_m.wasActivated()) {
    leftArrow_m.reload();
    currState_m = currState_m = !currState_m;
  } else if (rightArrow_m.wasActivated()) {
    rightArrow_m.reload();
    currState_m = currState_m = !currState_m;
  }
}