#include "GameOptions.h"

void GameOptions::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  target.draw(pauseBCG_m);
  target.draw(showCountdown_m);
  target.draw(showLevel_m);
  target.draw(simplifiedTurns_m);
}

GameOptions::GameOptions(Music& music)
    : pauseBCG_m(music, "pause before continue game", pauseBCGPosY_m,
                 pauseBeforeContinueGame_b),
      showCountdown_m(music, "show countdown", showCountdownPosY_m,
                      showCountdown_b),
      showLevel_m(music, "show classic game level", showLevelPosY_m,
                  showLevel_b),
      simplifiedTurns_m(music, "simplified turns", simplifiedTurnsPosY_m,
                        simplifiedTurns_b) {}

void GameOptions::handleEvents(sf::Event& event, sf::Vector2f& mousePos) {
  pauseBCG_m.handleEvents(event, mousePos);
  showCountdown_m.handleEvents(event, mousePos);
  showLevel_m.handleEvents(event, mousePos);
  simplifiedTurns_m.handleEvents(event, mousePos);
}

void GameOptions::reloadAfterOpenScene(sf::Vector2f& mousePos,
                                       bool wasReleased) {
  pauseBCG_m.reloadAfterOpenScene(mousePos, wasReleased);
  showCountdown_m.reloadAfterOpenScene(mousePos, wasReleased);
  showLevel_m.reloadAfterOpenScene(mousePos, wasReleased);
  simplifiedTurns_m.reloadAfterOpenScene(mousePos, wasReleased);
}

void GameOptions::saveData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::out |
                                          std::fstream::trunc);

  stream.write(reinterpret_cast<char*>(&pauseBeforeContinueGame_b),
               sizeof(pauseBeforeContinueGame_b));
  stream.write(reinterpret_cast<char*>(&showCountdown_b),
               sizeof(showCountdown_b));
  stream.write(reinterpret_cast<char*>(&showLevel_b), sizeof(showLevel_b));
  stream.write(reinterpret_cast<char*>(&simplifiedTurns_b),
               sizeof(simplifiedTurns_b));

  stream.close();
}

void GameOptions::readData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::in);

  stream.read(reinterpret_cast<char*>(&pauseBeforeContinueGame_b),
              sizeof(pauseBeforeContinueGame_b));
  stream.read(reinterpret_cast<char*>(&showCountdown_b),
              sizeof(showCountdown_b));
  stream.read(reinterpret_cast<char*>(&showLevel_b), sizeof(showLevel_b));
  stream.read(reinterpret_cast<char*>(&simplifiedTurns_b),
              sizeof(simplifiedTurns_b));

  stream.close();
}