#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <stack>

#include "Button.h"
#include "Globals.h"
#include "Music.h"
#include "MyText.h"
#include "MyWindow.h"
#include "TechInfo/TechInfo.h"

class Pause {
  static constexpr float buttonsOffset_m = 1.6f * cellSize_f;

  sf::Sound& backSoundRef_m;
  sf::Sound& setPopUpWindowSoundRef_m;

  static constexpr float windowWidth_m = viewWidthPix_f * 0.5f;
  static constexpr float windowHeight_m = viewHeightPix_f * 0.4f;
  static constexpr float windowPosX_m = viewWidthPix_f / 2.0f;
  static constexpr float windowPosY_m = viewHeightPix_f / 2.0f;
  sf::Texture windowTex_m;
  sf::Sprite windowSpr_m;

  /// screenshot
  sf::Texture screenshotTex_m;
  sf::Sprite screenshotSpr_m;

  /// label "Pause"
  static constexpr float pauseScale_m = 1.7f;
  static constexpr float pausePosX_m = windowWidth_m / 2.0f;
  static constexpr float pausePosY_m = windowHeight_m * 0.15f;
  // initialised by constructor
  MyText pause_m;

  /// button "resume"
  static constexpr float resumeScale_m = 1.0f;
  static constexpr float resumePosX_m = windowPosX_m;
  static constexpr float resumePosY_m = windowPosY_m - windowHeight_m * 0.02f;
  // initialised by constructor
  Button resume_m;

  /// button "restart"
  static constexpr float restartScale_m = resumeScale_m;
  static constexpr float restartPosX_m = windowPosX_m;
  static constexpr float restartPosY_m = resumePosY_m + buttonsOffset_m;
  // initialised by constructor
  Button restart_m;

  /// button "settings"
  static constexpr float settingsScale_m = resumeScale_m;
  static constexpr float settingsPosX_m = windowPosX_m;
  static constexpr float settingsPosY_m = resumePosY_m + buttonsOffset_m * 2.0f;
  // initialised by constructor
  Button settings_m;

  /// button "main menu"
  static constexpr float mainMenuScale_m = resumeScale_m;
  static constexpr float mainMenuPosX_m = windowPosX_m;
  static constexpr float mainMenuPosY_m = resumePosY_m + buttonsOffset_m * 3.0f;
  // initialised by constructor
  Button mainMenu_m;

  /// Handling events
  sf::Vector2f mousePos_m{0, 0};

  /// Additional
  sf::Event event_m{};
  float dTime_m = 0;

 public:
  explicit Pause(Music& music);
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
};