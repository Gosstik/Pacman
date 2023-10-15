#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "SettingsHelper.h"

class GraphicsOptions : public sf::Drawable {
  const std::string saveFilename_m = "../data/GraphicsOptions.info";

  /// option "show fps"
  float showFPSPosY_m = MMOptionStartPosY;
  // initialised by constructor
  BoolOption showFPS_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  explicit GraphicsOptions(Music& music);
  void handleEvents(sf::Event& event, sf::Vector2f& mousePos);
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased);

  void saveData();
  void readData();
};