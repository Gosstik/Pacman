#include "Portals.h"

void Portals::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto& p : portals_m) {
    target.draw(p.sprLeft);
    target.draw(p.sprRight);
  }
}

Portals::Portals() {
  tex_m.loadFromFile(blackSquare16ImageFilename_m);
  spr_m.setTexture(tex_m);
  spr_m.setOrigin(spr_m.getGlobalBounds().width / 2.0f,
                  spr_m.getGlobalBounds().height / 2.0f);
  spr_m.setScale(2.0f, 2.0f);
  portals_m.emplace_back(spr_m, leftPortalCenterPixX, leftPortalCenterPixY,
                         rightPortalCenterPixX, rightPortalCenterPixY);
}