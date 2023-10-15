#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <stack>

#include "Button.h"
#include "ComingSoon.h"
#include "Globals.h"
#include "MyText.h"
#include "MyWindow.h"
#include "PacmanTitle.h"
#include "TechInfo/TechInfo.h"

class MainMenu {
  static constexpr float buttonsOffset_m = 2.0f * cellSize_f;
  static constexpr float buttonsScale_m = 1.2f;
  PacmanTitle pacmanTitle_m;

  /// label "main menu"
  static constexpr float mainMenuScale_m = 2.0f;
  static constexpr float mainMenuPosX_m = viewWidthPix_f * 0.5f;
  static constexpr float mainMenuPosY_m = viewHeightPix_f * 0.35f;
  // initialised by constructor
  MyText mainMenu_m;

  /// button "start game"
  static constexpr float startGamePosX_m = viewWidthPix_f * 0.5f;
  static constexpr float startGamePosY_m = viewHeightPix_f * 0.6f;
  // initialised by constructor
  Button startGame_m;

  /// button "choose mode"
  static constexpr float chooseModePosX_m = viewWidthPix_f * 0.5f;
  static constexpr float chooseModePosY_m =
      startGamePosY_m + buttonsScale_m * buttonsOffset_m;
  // initialised by constructor
  Button chooseMode_m;

  /// button "profile"
  static constexpr float profilesPosX_m = viewWidthPix_f * 0.5f;
  static constexpr float profilesPosY_m =
      startGamePosY_m + buttonsScale_m * buttonsOffset_m * 2.0f;
  // initialised by constructor
  Button profiles_m;

  /// button "settings"
  static constexpr float settingsPosX_m = viewWidthPix_f * 0.5f;
  static constexpr float settingsPosY_m =
      startGamePosY_m + buttonsScale_m * buttonsOffset_m * 3.0f;
  // initialised by constructor
  Button settings_m;

  /// button "exit"
  float exitPosX_m = viewWidthPix_f * 0.5f;
  float exitPosY_m = startGamePosY_m + buttonsScale_m * buttonsOffset_m * 4.0f;
  // initialised by constructor
  Button exit_m;

  /// Handling events
  sf::Vector2f mousePos_m{0, 0};
  sf::Event event_m{};

  /// Additional
  //    sf::Clock clock_m;
  float dTime_m = 0;
  ComingSoon comingSoon_m;
  sf::Sound& MMSoundtrackRef_m;

 public:
  explicit MainMenu(Music& music);
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

  static void setBoolMode();
};