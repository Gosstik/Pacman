#include "Cycles.h"

void Cycles::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (showCycles_b) {
    target.draw(text_m);
    target.draw(value_m);
  }
}

Cycles::Cycles(float scale, float posPixX, float posPixY) : scale_m(scale) {
  text_m.setString("cycles:");
  text_m.setPosition(posPixX, posPixY);
  text_m.setScale(scale_m, scale_m);

  value_m.setPosition(
      text_m.getGlobalBounds().left + text_m.getGlobalBounds().width,
      text_m.getGlobalBounds().top);
  value_m.setScale(scale_m, scale_m);

  setRate(rateValue_m);  // todo: pass to the constructor
}

void Cycles::reloadRun() {
  accumTimeOneRate_m -= constTimeOneRate_m;
  ratesCount_m += 1.0f;
}

void Cycles::addToAccumTime(float dTime) {
  accumTimeOneRate_m += dTime;
  accumTimeUpdateDisplayValue_m += dTime;
}

void Cycles::updateDisplayValue() {
  displayValue_m =
      int(mcsInSec * (ratesCount_m + accumTimeOneRate_m / constTimeOneRate_m) /
          accumTimeUpdateDisplayValue_m);
  accumTimeUpdateDisplayValue_m = accumTimeOneRate_m;
  ratesCount_m = 0.0f;
  value_m.setString(displayValue_m);
}

void Cycles::setRate(int value) {
  if (value > 75)
    rateValue_m = value;
  else
    rateValue_m = 75;
  float i = 0.09;
  float coef = 0.9;
  constTimeOneRate_m = mcsInSec / float(rateValue_m) * coef;
  while (i > 0 && mcsInSec / constTimeOneRate_m - float(rateValue_m) > 0.01f) {
    constTimeOneRate_m = mcsInSec / float(rateValue_m) * coef;
    coef += i;
    i /= 10.0f;
  }
}

bool Cycles::run() const { return accumTimeOneRate_m >= constTimeOneRate_m; }

float Cycles::diffTime() const {
  return constTimeOneRate_m - accumTimeOneRate_m;
}

float Cycles::getOneRateDuration() const { return constTimeOneRate_m; }