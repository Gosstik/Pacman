#pragma once

#include "Music.h"
#include "MyText.h"
#include "SettingsHelper.h"

class Volume : public sf::Drawable {
 protected:
  enum SliderState { idle, hover, active };
  static constexpr float lineWidth_m = cellSize_f * 18.0f;
  static constexpr float lineHeight_m = cellSize_f / 4.0f;
  static constexpr float lineOutlineThickness_m = cellSize_f / 16.0f;
  static constexpr float sliderWidth_m = lineWidth_m * 0.05f;
  static constexpr float sliderHeight_m = sliderWidth_m * 1.6f;
  static constexpr float sliderIdleOutlineThickness_m = cellSize_f / 16.0f;
  static constexpr float sliderHoverOutlineThickness_m =
      sliderIdleOutlineThickness_m * 2.0f;
  static const sf::Color outlineColor_m;
  static const sf::Color lineFillColor_m;
  static const sf::Color sliderIdleFillColor_m;
  static const sf::Color sliderActiveFillColor_m;

  Music::SoundKind soundKind_m{};  // defines by successors

  SliderState currState_m = idle;
  SliderState newState_m = idle;

  Music& musicRef_m;
  sf::Sound& hoverSoundRef_m;
  sf::Sound& pressSoundRef_m;
  sf::Sound& releaseSoundRef_m;

  float linePosY_m;
  float lineLeftBorder_m;
  float lineRightBorder_m;

  MyText name_m;
  sf::RectangleShape line_m;
  sf::RectangleShape slider_m;

  bool wasReleased_m = true;

  float mousePosOffsetX_m = 0;
  float mousePosOffsetY_m = 0;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(name_m);
    target.draw(line_m);
    target.draw(slider_m);
  }

 public:
  Volume(Music& music, const std::string& name, float scale, float namePosX,
         float namePosY, float lineRightPosX);

  virtual void handleEvents(sf::Event& event, sf::Vector2f& mousePos);
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased);
  void setState(const SliderState& state, bool playSounds = true);
  virtual void saveData(std::fstream& stream) = 0;
  virtual void readData(std::fstream& stream) = 0;
};

struct MusicVolume : public Volume {
  MusicVolume(Music& music, float scale, float namePosX, float namePosY,
              float lineRightPosX)
      : Volume(music, "music volume", scale, namePosX, namePosY,
               lineRightPosX) {
    slider_m.setPosition(lineLeftBorder_m + 0.01f *
                                                musicRef_m.getMusicVolume() *
                                                line_m.getGlobalBounds().width,
                         linePosY_m);
    soundKind_m = Music::SoundKind::music;
  }
  void saveData(std::fstream& stream) override {
    float volume = musicRef_m.getMusicVolume();
    stream.write(reinterpret_cast<char*>(&volume), sizeof(volume));
  }
  void readData(std::fstream& stream) override {
    float volume;
    stream.read(reinterpret_cast<char*>(&volume), sizeof(volume));
    musicRef_m.setVolume(Music::SoundKind::music, volume);
    slider_m.setPosition(
        lineLeftBorder_m + 0.01f * volume * line_m.getGlobalBounds().width,
        linePosY_m);
  }
};

struct EffectsVolume : public Volume {
  EffectsVolume(Music& music, float scale, float namePosX, float namePosY,
                float lineRightPosX)
      : Volume(music, "effects volume", scale, namePosX, namePosY,
               lineRightPosX) {
    slider_m.setPosition(lineLeftBorder_m + 0.01f *
                                                musicRef_m.getEffectVolume() *
                                                line_m.getGlobalBounds().width,
                         linePosY_m);
    soundKind_m = Music::SoundKind::effect;
  }
  void saveData(std::fstream& stream) override {
    float volume = musicRef_m.getEffectVolume();
    stream.write(reinterpret_cast<char*>(&volume), sizeof(volume));
  }
  void readData(std::fstream& stream) override {
    float volume;
    stream.read(reinterpret_cast<char*>(&volume), sizeof(volume));
    musicRef_m.setVolume(Music::SoundKind::effect, volume);
    slider_m.setPosition(
        lineLeftBorder_m + 0.01f * volume * line_m.getGlobalBounds().width,
        linePosY_m);
  }
};