#pragma once

#include "Globals.h"

struct PacmanTitle : public sf::Sprite {
 private:
  static const std::string imageFilename_m;

  float pacmanTitlePosX_m = viewWidthPix_f * 0.5f;
  float pacmanTitlePosY_m = viewHeightPix_f * 0.13f;

  // initialised by constructor
  sf::Image image_m;
  sf::Texture texture_m;

 public:
  PacmanTitle() {
    image_m.loadFromFile(imageFilename_m);
    image_m.createMaskFromColor(sf::Color::Black);
    texture_m.loadFromImage(image_m);
    setTexture(texture_m);
    setOrigin(float(getLocalBounds().width) / 2.0f,
              float(getLocalBounds().height) / 2.0f);
    setPosition(pacmanTitlePosX_m, pacmanTitlePosY_m);
    float coef = 0.8f * viewWidthPix_f / getLocalBounds().width;
    setScale(coef, coef);
  }
};