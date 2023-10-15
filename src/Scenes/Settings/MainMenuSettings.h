#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <stack>

#include "Button.h"
#include "ComingSoon.h"
#include "DevOptions.h"
#include "GameOptions.h"
#include "Globals.h"
#include "GraphicsOptions.h"
#include "MusicOptions.h"
#include "MyText.h"
#include "MyWindow.h"
#include "PacmanTitle.h"
#include "SettingHeaderButton.h"
#include "SettingsHelper.h"
#include "TechInfo/TechInfo.h"

class MainMenuSettings {
  enum SettingsHeader { game = 0, music, graphics, dev };
  SettingsHeader currHeader_m = game;
  sf::Sound& backSoundRef_m;

  static constexpr float headersStartPosX_m =
      viewWidthPix_f * 0.05 + (viewWidthPix_f * 0.9) / 8.0f;
  static constexpr float headersOffsetX_m = (viewWidthPix_f * 0.9) / 4.0f;
  static constexpr float headersPosY_m = viewHeightPix_f * 0.3f;

  //    PacmanTitle pacmanTitle_m;

  /// label "settings"
  float settingsScale_m = 2.0f;
  float settingsPosX_m = viewWidthPix_f * 0.5f;
  float settingsPosY_m = viewHeightPix_f * 0.15f;
  // initialised by constructor
  MyText settings_m;

  float gamePosX_m = headersStartPosX_m;
  float musicPosX = headersStartPosX_m + headersOffsetX_m;
  float graphicsPosX = headersStartPosX_m + headersOffsetX_m * 2;
  float devPosX = headersStartPosX_m + headersOffsetX_m * 3;

  SettingHeaderButton gameButton_m;
  SettingHeaderButton musicButton_m;
  SettingHeaderButton graphicsButton_m;
  SettingHeaderButton devButton_m;

  GameOptions gameOptions_m;
  MusicOptions musicOptions_m;
  GraphicsOptions graphicsOptions_m;
  DevOptions devOptions_m;

  /// Handling events
  sf::Vector2f mousePos_m{0, 0};
  sf::Event event_m{};

  /// Additional
  float dTime_m = 0;
  bool cameFromCloseWindow_m = false;
  //    ComingSoon comingSoon_m

 public:
  explicit MainMenuSettings(Music& music);
  ~MainMenuSettings();
  void open(MyWindow& window,
            std::stack<std::reference_wrapper<bool>>& prevScene,
            TechInfo& techInfo);
  void saveData();
  void readData();

 private:
  void handleEvents(MyWindow& window,
                    std::stack<std::reference_wrapper<bool>>& prevScene);
  void reloadAfterOpen(MyWindow& window, TechInfo& techInfo);
  template <class T>
  void displayObjects(T& window, TechInfo& techInfo, bool makeScreenshot);
  void makeScreenshot(MyWindow& window, TechInfo& techInfo);

  void deactivateCurrOptionButton();
};