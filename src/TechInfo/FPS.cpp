#include "FPS.h"

void FPS::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (showFPS_b) {
    target.draw(text_m);
    target.draw(value_m);
  }
}

FPS::FPS(float scale, float posPixX, float posPixY) : scale_m(scale) {
  text_m.setString("fps:");
  text_m.setPosition(posPixX, posPixY);
  text_m.setScale(scale_m, scale_m);

  value_m.setPosition(
      text_m.getGlobalBounds().left + text_m.getGlobalBounds().width,
      text_m.getGlobalBounds().top);
  value_m.setScale(scale_m, scale_m);

  setRate(rateValue_m);  // todo: pass to the constructor
}

FPS::FPS() {
  text_m.setString("fps:");
  text_m.setPosition(0, 0);
  text_m.setScale(scale_m, scale_m);

  value_m.setPosition(
      text_m.getGlobalBounds().left + text_m.getGlobalBounds().width,
      text_m.getGlobalBounds().top);
  value_m.setScale(scale_m, scale_m);

  setRate(rateValue_m);
}

void FPS::addToAccumTime(float dTime) {
  accumTimeOneRate_m += dTime;
  accumTimeUpdateDisplayValue_m += dTime;
}

void FPS::updateDisplayValue() {
  displayValue_m =
      int(mcsInSec * (ratesCount_m + accumTimeOneRate_m / constTimeOneRate_m) /
          accumTimeUpdateDisplayValue_m);
  accumTimeUpdateDisplayValue_m = accumTimeOneRate_m;
  ratesCount_m = 0.0f;
  value_m.setString(displayValue_m);
}

bool FPS::run() const { return accumTimeOneRate_m >= constTimeOneRate_m; }

void FPS::reloadRun() {
  while (accumTimeOneRate_m >= constTimeOneRate_m)
    accumTimeOneRate_m -= constTimeOneRate_m;
  ratesCount_m += 1.0F;
}

void FPS::setRate(int value) {
  rateValue_m = value;  // num of fps
  float i = 0.09;
  float coef = 0.9;
  constTimeOneRate_m = mcsInSec / float(rateValue_m) * coef;
  while (i > 0 && mcsInSec / constTimeOneRate_m - float(rateValue_m) > 0.01f) {
    constTimeOneRate_m = mcsInSec / float(rateValue_m) * coef;
    coef += i;
    i /= 10.0f;
  }
}

float FPS::diffTime() const { return constTimeOneRate_m - accumTimeOneRate_m; }