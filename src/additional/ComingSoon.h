#pragma once

#include "Globals.h"
#include "MyText.h"

struct ComingSoon : public sf::Drawable {
  static constexpr float constTimeDisplay = 2000000.0f;
  static constexpr float posX_m = viewWidthPix_f * 0.5f;
  static constexpr float posY_m = viewHeightPix_f * 0.95f;
  static constexpr float scale_m = 1.0f;

  bool isShown_m = false;
  float accumTimeDisplay_m = 0;
  MyText text_m;

  ComingSoon() : text_m("coming soon", scale_m, scale_m, posX_m, posY_m, true) {
    text_m.setColor(sf::Color::Red);
  }

  void show(bool b) {
    isShown_m = b;
    accumTimeDisplay_m = 0;
  }

  void update(float dTime) {
    if (isShown_m) {
      accumTimeDisplay_m += dTime;
      if (accumTimeDisplay_m >= constTimeDisplay) {
        isShown_m = false;
        accumTimeDisplay_m = 0;
      }
    }
  }

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    if (isShown_m) target.draw(text_m);
  }
};