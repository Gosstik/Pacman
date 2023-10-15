#pragma once

#include <vector>

#include "Globals.h"

class Portals : public sf::Drawable {
  struct Portal {
    float xLeft = 0;
    float yLeft = 0;
    float xRight = 0;
    float yRight = 0;
    sf::Sprite sprLeft;
    sf::Sprite sprRight;
    Portal(sf::Sprite& spr, float xLeft_, float yLeft_, float xRight_,
           float yRight_)
        : xLeft(xLeft_), yLeft(yLeft_), xRight(xRight_), yRight(yRight_) {
      sprRight = sprLeft = spr;
      sprLeft.setPosition(xLeft, yLeft);
      sprRight.setPosition(xRight, yRight);
    }
  };

  sf::Image img_m;
  sf::Texture tex_m;
  sf::Sprite spr_m;
  std::vector<Portal> portals_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  Portals();
};