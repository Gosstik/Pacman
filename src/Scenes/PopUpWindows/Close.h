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

class Close {
  sf::Sound& setPopUpWindowSoundRef_m;

  static constexpr float windowWidth_m = viewWidthPix_f * 0.7f;
  static constexpr float windowHeight_m = viewHeightPix_f * 0.35f;
  static constexpr float windowPosX_m = viewWidthPix_f / 2.0f;
  static constexpr float windowPosY_m = viewHeightPix_f / 2.0f;
  sf::Texture windowTex_m;
  sf::Sprite windowSpr_m;

  /// screenshot
  sf::Texture screenshotTex_m;
  sf::Sprite screenshotSpr_m;

  /// label "?"
  static constexpr float questScale_m = 4.5f;
  static constexpr float questPosX_m = windowWidth_m * 0.15f;
  static constexpr float questPosY_m = windowHeight_m * 0.3f;
  // initialised by constructor
  MyText quest_m;

  /// label "exit game?"
  static constexpr float text1Scale_m = 1.2f;
  static constexpr float text1PosX_m = windowWidth_m * 0.3f;
  static constexpr float text1PosY_m = windowHeight_m * 0.15f;
  // initialised by constructor
  MyText text1_m;

  /// label "unsaved data will"
  static constexpr float text2Scale_m = 1.0f;
  static constexpr float text2PosX_m = windowWidth_m * 0.3f;
  static constexpr float text2PosY_m = windowHeight_m * 0.4f;
  sf::Color text2Color_m = sf::Color::Red;
  // initialised by constructor
  MyText text2_m;

  /// label "be lost"
  static constexpr float text3Scale_m = text2Scale_m;
  static constexpr float text3PosX_m = text2PosX_m;
  static constexpr float text3PosY_m =
      text2PosY_m + cellSize_f * text2Scale_m * 1.4f;
  sf::Color text3Color_m = text2Color_m;
  // initialised by constructor
  MyText text3_m;

  /// button "yes"
  static constexpr float yesScale_m = 1.0f;
  static constexpr float yesPosX_m = windowPosX_m;
  static constexpr float yesPosY_m = windowPosY_m + windowHeight_m * 0.3f;
  // initialised by constructor
  Button yes_m;

  /// button "no"
  static constexpr float noScale_m = yesScale_m;
  static constexpr float noPosX_m = windowPosX_m + windowWidth_m * 0.3f;
  static constexpr float noPosY_m = windowPosY_m + windowHeight_m * 0.3f;
  // initialised by constructor
  Button no_m;

  /// sound effects
  sf::Sound backSound_m;

  /// Handling events
  sf::Vector2f mousePos_m{0, 0};
  sf::Event event_m{};

  /// Additional
  float dTime_m = 0;

 public:
  explicit Close(Music& music);
  void open(MyWindow& window,
            std::stack<std::reference_wrapper<bool>>& prevScene,
            TechInfo& techInfo);

 private:
  void reloadAfterOpen(MyWindow& window, TechInfo& techInfo);
  void handleEvents(MyWindow& window,
                    std::stack<std::reference_wrapper<bool>>& prevScene);
  template <class T>
  void displayObjects(T& window, TechInfo& techInfo,
                      bool makeScreenshot = false);
  void makeScreenshot(MyWindow& window, TechInfo& techInfo);
};