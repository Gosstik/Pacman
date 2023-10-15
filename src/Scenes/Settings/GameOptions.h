#pragma once

#include <SFML/Graphics.hpp>

#include "SettingsHelper.h"

class GameOptions : public sf::Drawable {
  const std::string saveFilename_m = "../data/GameOptions.info";

  /// option "pause before continue game"
  float pauseBCGPosY_m = MMOptionStartPosY;
  // initialised by constructor
  BoolOption pauseBCG_m;

  /// option "show countdown"
  float showCountdownPosY_m = MMOptionStartPosY + MMButtonsOffset;
  // initialised by constructor
  BoolOption showCountdown_m;

  /// option "show classic game level"
  float showLevelPosY_m = MMOptionStartPosY + MMButtonsOffset * 2;
  // initialised by constructor
  BoolOption showLevel_m;

  /// option "simplified turns"
  float simplifiedTurnsPosY_m = MMOptionStartPosY + MMButtonsOffset * 3;
  // initialised by constructor
  BoolOption simplifiedTurns_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  explicit GameOptions(Music& music);
  void handleEvents(sf::Event& event, sf::Vector2f& mousePos);
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased);

  void saveData();
  void readData();
};