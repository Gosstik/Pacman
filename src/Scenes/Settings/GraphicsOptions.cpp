#include "GraphicsOptions.h"

void GraphicsOptions::draw(sf::RenderTarget& target,
                           sf::RenderStates states) const {
  target.draw(showFPS_m);
}

GraphicsOptions::GraphicsOptions(Music& music)
    : showFPS_m(music, "show fps", showFPSPosY_m, showFPS_b) {}

void GraphicsOptions::handleEvents(sf::Event& event, sf::Vector2f& mousePos) {
  showFPS_m.handleEvents(event, mousePos);
}

void GraphicsOptions::reloadAfterOpenScene(sf::Vector2f& mousePos,
                                           bool wasReleased) {
  showFPS_m.reloadAfterOpenScene(mousePos, wasReleased);
}

void GraphicsOptions::saveData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::out |
                                          std::fstream::trunc);

  stream.write(reinterpret_cast<char*>(&showFPS_b), sizeof(showFPS_b));

  stream.close();
}

void GraphicsOptions::readData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::in);

  stream.read(reinterpret_cast<char*>(&showFPS_b), sizeof(showFPS_b));

  stream.close();
}