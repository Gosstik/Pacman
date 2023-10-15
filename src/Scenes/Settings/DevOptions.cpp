#include "DevOptions.h"

void DevOptions::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(showCycles_m);
  target.draw(showEntityHB_m);
  target.draw(showButtonsHB_m);
  target.draw(showGhostsTargets_m);
}

DevOptions::DevOptions(Music& music)
    : showCycles_m(music, "show cycles", showCyclesPosY_m, showCycles_b),
      showEntityHB_m(music, "show entity hitbox", showEntityHBPosY_m,
                     showEntityHitbox_b),
      showButtonsHB_m(music, "show buttons hitbox", showButtonsHBPosY_m,
                      showButtonsHitbox_b),
      showGhostsTargets_m(music, "show ghosts targets", showGhostsTargetsPosY_m,
                          showGhostsTargets_b) {}

void DevOptions::handleEvents(sf::Event& event, sf::Vector2f& mousePos) {
  showCycles_m.handleEvents(event, mousePos);
  showEntityHB_m.handleEvents(event, mousePos);
  showButtonsHB_m.handleEvents(event, mousePos);
  showGhostsTargets_m.handleEvents(event, mousePos);
}

void DevOptions::reloadAfterOpenScene(sf::Vector2f& mousePos,
                                      bool wasReleased) {
  showCycles_m.reloadAfterOpenScene(mousePos, wasReleased);
  showEntityHB_m.reloadAfterOpenScene(mousePos, wasReleased);
  showButtonsHB_m.reloadAfterOpenScene(mousePos, wasReleased);
  showGhostsTargets_m.reloadAfterOpenScene(mousePos, wasReleased);
}

void DevOptions::saveData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::out |
                                          std::fstream::trunc);

  stream.write(reinterpret_cast<char*>(&showCycles_b), sizeof(showCycles_b));
  stream.write(reinterpret_cast<char*>(&showEntityHitbox_b),
               sizeof(showEntityHitbox_b));
  stream.write(reinterpret_cast<char*>(&showButtonsHitbox_b),
               sizeof(showButtonsHitbox_b));
  stream.write(reinterpret_cast<char*>(&showGhostsTargets_b),
               sizeof(showGhostsTargets_b));

  stream.close();
}

void DevOptions::readData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::in);

  stream.read(reinterpret_cast<char*>(&showCycles_b), sizeof(showCycles_b));
  stream.read(reinterpret_cast<char*>(&showEntityHitbox_b),
              sizeof(showEntityHitbox_b));
  stream.read(reinterpret_cast<char*>(&showButtonsHitbox_b),
              sizeof(showButtonsHitbox_b));
  stream.read(reinterpret_cast<char*>(&showGhostsTargets_b),
              sizeof(showGhostsTargets_b));

  stream.close();
}