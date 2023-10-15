#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <stack>

#include "Button.h"
#include "ComingSoon.h"
#include "Globals.h"
#include "Music.h"
#include "MyText.h"
#include "MyWindow.h"
#include "PacmanTitle.h"
#include "PillButton.h"
#include "TechInfo/TechInfo.h"

class ChooseMode {
  static constexpr float buttonsOffset_m = 5.0f;
  static constexpr float buttonsScale_m = 1.2f;

  /// image "pacmanTitle"
  PacmanTitle pacmanTitle_m;

  /// label "choose mode"
  static constexpr float chooseModeScale_m = 2.0f;
  static constexpr float chooseModePosX_m = viewWidthPix_f * 0.5f;
  static constexpr float chooseModePosY_m = viewHeightPix_f * 0.35f;
  // initialised by constructor
  MyText chooseMode_m;

  /// button "classic game"
  static constexpr float classicGamePosX_m = viewWidthPix_f * 0.25f;
  static constexpr float classicGamePosY_m = viewHeightPix_f * 0.65f;
  // initialised by constructor
  PillButton classicGame_m;

  /// button "endless map"
  static constexpr float endlessMapPosX_m = viewWidthPix_f * 0.75f;
  static constexpr float endlessMapPosY_m = classicGamePosY_m;
  // initialised by constructor
  PillButton endlessMap_m;

  /// button "1up"
  static constexpr float oneUpPosX_m = viewWidthPix_f * 0.2f;
  static constexpr float oneUpPosY_m =
      classicGamePosY_m + buttonsScale_m * cellSize_f * buttonsOffset_m;
  // initialised by constructor
  PillButton oneUp_m;

  /// button "2up"
  static constexpr float twoUpPosX_m = viewWidthPix_f * 0.5f;
  static constexpr float twoUpPosY_m = oneUpPosY_m;
  // initialised by constructor
  PillButton twoUp_m;

  /// button "network"
  static constexpr float networkPosX_m = viewWidthPix_f * 0.8f;
  static constexpr float networkPosY_m = oneUpPosY_m;
  // initialised by constructor
  PillButton network_m;

  /// Handling events
  sf::Vector2f mousePos_m{0, 0};
  sf::Event event_m{};

  /// Additional
  float dTime_m = 0;
  ComingSoon comingSoon_m;
  sf::Sound& backSoundRef_m;

 public:
  explicit ChooseMode(Music& music);
  void open(MyWindow& window,
            std::stack<std::reference_wrapper<bool>>& prevScene,
            TechInfo& techInfo);

 private:
  void reloadAfterOpen(MyWindow& window, TechInfo& techInfo);
  void handleEvents(MyWindow& window,
                    std::stack<std::reference_wrapper<bool>>& prevScene);
  template <class T>
  void displayObjects(T& window, TechInfo& techInfo, bool makeScreenshot);
  void makeScreenshot(MyWindow& window, TechInfo& techInfo);

  void deactivateCurrMode();
  void deactivateCurrUp();
};