#include "TechInfo.h"

void TechInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(fps_m);
  target.draw(cycles_m);
}

TechInfo::TechInfo()
    : fps_m(scale_m, fpsPosPixX_m, fpsPosPixY_m),
      cycles_m(scale_m, cyclesPosPixX_m, cyclesPosPixY_m) {}

void TechInfo::update(float dTime) {
  accumTimeUpdateDisplayValues_m += dTime;
  fps_m.addToAccumTime(dTime);
  cycles_m.addToAccumTime(dTime);
  if (accumTimeUpdateDisplayValues_m >= updateFrequency_m) {
    fps_m.updateDisplayValue();
    cycles_m.updateDisplayValue();
    while (accumTimeUpdateDisplayValues_m >= updateFrequency_m)
      accumTimeUpdateDisplayValues_m -= updateFrequency_m;
  }
}

void TechInfo::handleTime() {
  if (accumTimeUpdateDisplayValues_m >= updateFrequency_m) {
    fps_m.updateDisplayValue();
    cycles_m.updateDisplayValue();
    while (accumTimeUpdateDisplayValues_m >= updateFrequency_m)
      accumTimeUpdateDisplayValues_m -= updateFrequency_m;
  }
  dTime_m = float(clock_m.restart().asMicroseconds());
//  std::cout << dTime_m << '\n';
  float tmpTime =
      std::min(fps_m.diffTime() - dTime_m, cycles_m.diffTime() - dTime_m);
  while (float(clock_m.getElapsedTime().asMicroseconds()) < tmpTime) {
  }
  dTime_m += float(clock_m.restart().asMicroseconds());
  fps_m.addToAccumTime(dTime_m);
  cycles_m.addToAccumTime(dTime_m);
  accumTimeUpdateDisplayValues_m += dTime_m;
}

// void TechInfo::setFrameRate(int value) {
//     fps_m.setRate(value);
//     cycles_m.setRate(value);
// }

void TechInfo::reloadRunDisplay() { fps_m.reloadRun(); }

void TechInfo::reloadRunCycle() { cycles_m.reloadRun(); }

// void TechInfo::reloadClock() {
//     clock_m.restart();
// }

bool TechInfo::runDisplay() const { return fps_m.run(); }

bool TechInfo::runCycle() const { return cycles_m.run(); }

float TechInfo::getCycleDuration() const {
  return cycles_m.getOneRateDuration();
}