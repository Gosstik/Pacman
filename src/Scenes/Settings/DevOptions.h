#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "SettingsHelper.h"

class DevOptions : public sf::Drawable {
  const std::string saveFilename_m = "../data/DevOptions.info";

  /// option "show cycles"
  float showCyclesPosY_m = MMOptionStartPosY;
  // initialised by constructor
  BoolOption showCycles_m;

  /// option "show entity hitbox"
  float showEntityHBPosY_m = MMOptionStartPosY + MMButtonsOffset;
  // initialised by constructor
  BoolOption showEntityHB_m;

  /// option "show buttons hitbox"
  float showButtonsHBPosY_m = MMOptionStartPosY + MMButtonsOffset * 2;
  // initialised by constructor
  BoolOption showButtonsHB_m;

  /// option "show ghosts targets"
  float showGhostsTargetsPosY_m = MMOptionStartPosY + MMButtonsOffset * 3;
  // initialised by constructor
  BoolOption showGhostsTargets_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  explicit DevOptions(Music& music);
  void handleEvents(sf::Event& event, sf::Vector2f& mousePos);
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased);

  void saveData();
  void readData();
};