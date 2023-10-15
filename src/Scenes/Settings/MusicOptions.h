#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "MusicOptionsHelper.h"
#include "SettingsHelper.h"

class MusicOptions : public sf::Drawable {
  const std::string saveFilename_m = "../data/MusicOptions.info";

  sf::Sound& MMSoundtrack_m;

  /// option "musicVolume"
  float musicVolumeNamePosY_m = MMOptionStartPosY;
  float musicVolumeScale_m = 1.0f;
  // initialised by constructor
  MusicVolume musicVolume_m;

  /// option "musicVolume"
  float effectsVolumeNamePosY_m = MMOptionStartPosY + MMButtonsOffset;
  float effectsVolumeScale_m = 1.0f;
  // initialised by constructor
  EffectsVolume effectsVolume_m;

  /// option "play music in main menu"
  float playMusicInMMPosY_m = MMOptionStartPosY + MMButtonsOffset * 2;
  // initialised by constructor
  BoolOption playMusicInMM_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  explicit MusicOptions(Music& music);
  void handleEvents(sf::Event& event, sf::Vector2f& mousePos);
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased);

  void saveData();
  void readData();
};