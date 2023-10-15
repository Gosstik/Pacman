#pragma once

#include "MyText.h"

class FPS : public sf::Drawable {
  int rateValue_m = 1000;  // num of fps
  float constTimeOneRate_m;
  float accumTimeOneRate_m = 0;

  int displayValue_m = 0;
  float accumTimeUpdateDisplayValue_m = 0;
  float ratesCount_m = 0;

  float scale_m;
  MyText text_m;
  MyText value_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  FPS(float scale, float posPixX, float posPixY);
  FPS();
  void reloadRun();
  void addToAccumTime(float dTime);
  void updateDisplayValue();
  void setRate(int value);

  bool run() const;
  float diffTime() const;
};