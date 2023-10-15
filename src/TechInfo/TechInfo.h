#pragma once

#include "Cycles.h"
#include "FPS.h"
#include "MyText.h"

class TechInfo : public sf::Drawable {
  static constexpr float updateFrequency_m = 0.3f * mcsInSec;
  static constexpr float scale_m = 0.5f;

  static constexpr float fpsPosPixX_m = 0;
  static constexpr float fpsPosPixY_m = 0;
  static constexpr float cyclesPosPixX_m = fpsPosPixX_m;
  static constexpr float cyclesPosPixY_m =
      fpsPosPixY_m + cellSize_f * scale_m * 1.2f;

  float accumTimeUpdateDisplayValues_m = 0;

  sf::Clock clock_m;
  float dTime_m = 0;

  FPS fps_m;
  Cycles cycles_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  TechInfo();
  void update(float dTime);
  void handleTime();
  //    void setFrameRate(int value);

  void reloadRunDisplay();
  void reloadRunCycle();
  //    void reloadClock();
  bool runDisplay() const;
  bool runCycle() const;
  float getCycleDuration() const;
};