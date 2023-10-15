#pragma once

#include <SFML/Graphics.hpp>

#include "ClassicGameMusic.h"
#include "Globals.h"
#include "Intermission1.h"
#include "Intermission2.h"
#include "Intermission3.h"

struct Intermissions : public sf::Drawable {
 public:
  sf::Image image_m;
  sf::Texture texture_m;

  float rectWidth_m = 4 * cellSize_f;
  float rectHeight_m = viewHeightPix_f;
  float leftRectPosPixX_m = rectWidth_m / 2.0f;
  float leftRectPosPixY_m = viewHeightPix_f / 2.0f;
  float rightRectPosPixX_m = viewWidthPix_f - rectWidth_m / 2.0f;
  float rightRectPosPixY_m = viewHeightPix_f / 2.0f;
  sf::RectangleShape leftBlackRect_m;
  sf::RectangleShape rightBlackRect_m;

  float leftRectBorderPixX_m = leftRectPosPixX_m + rectWidth_m / 2.0f;
  float rightRectBorderPixX_m = rightRectPosPixX_m - rectWidth_m / 2.0f;

  size_t curr = 0;

  Intermission1 inter1_m;
  Intermission2 inter2_m;
  Intermission3 inter3_m;

  explicit Intermissions(ClassicGameMusic& CGMusic)
      : inter1_m(texture_m, leftRectBorderPixX_m, rightRectBorderPixX_m,
                 Music::getIntermissionDuration() * 2),
        inter2_m(texture_m, leftRectBorderPixX_m, rightRectBorderPixX_m),
        inter3_m(texture_m, leftRectBorderPixX_m, rightRectBorderPixX_m) {
    image_m.loadFromFile("../Images/intermissions16.png");
    image_m.createMaskFromColor(sf::Color::Black);
    texture_m.loadFromImage(image_m);

    leftBlackRect_m.setSize({rectWidth_m, rectHeight_m});
    leftBlackRect_m.setOrigin(leftBlackRect_m.getLocalBounds().width / 2.0f,
                              leftBlackRect_m.getLocalBounds().height / 2.0f);
    leftBlackRect_m.setFillColor(sf::Color::Black);
    leftBlackRect_m.setPosition(leftRectPosPixX_m, leftRectPosPixY_m);

    rightBlackRect_m.setSize({rectWidth_m, rectHeight_m});
    rightBlackRect_m.setOrigin(rightBlackRect_m.getLocalBounds().width / 2.0f,
                               rightBlackRect_m.getLocalBounds().height / 2.0f);
    rightBlackRect_m.setFillColor(sf::Color::Black);
    rightBlackRect_m.setPosition(rightRectPosPixX_m, rightRectPosPixY_m);
  }

  void updateCurrent(ClassicGameMusic& CGMusic, float dTime) {
    if (curr == 0) {
      inter1_m.update(CGMusic, dTime);
      if (inter1_m.ended()) curr = 1;
    } else if (curr == 1) {
      inter2_m.update(CGMusic, dTime);
      if (inter2_m.ended()) curr = 2;
    } else {
      inter3_m.update(CGMusic, dTime);
      if (inter3_m.ended()) curr = 0;
    }
  }

  void reloadCurrent(ClassicGameMusic& CGMusic) {
    if (curr == 0)
      inter1_m.reload(CGMusic);
    else if (curr == 1)
      inter2_m.reload(CGMusic);
    else
      inter3_m.reload(CGMusic);
  }

  bool currentEnded() const {
    if (curr == 0)
      return inter1_m.ended();
    else if (curr == 1)
      return inter2_m.ended();
    else
      return inter3_m.ended();
  }

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    if (curr == 0) {
      target.draw(inter1_m);
    } else if (curr == 1) {
      target.draw(inter2_m);
    } else {
      target.draw(inter3_m);
    }
    target.draw(leftBlackRect_m);
    target.draw(rightBlackRect_m);
  }
};