#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Button.h"
#include "MyText.h"

static constexpr float MMOptionNamePosX =
    viewWidthPix_f * 0.05f;  // MM == Main Menu
static constexpr float optionsScale = 1.0f;
static constexpr float arrowsScale = 1.3f;
static constexpr float MMExtremeRightPosX =
    viewWidthPix_f * 0.97f;  // MM == Main Menu
static constexpr float rightArrowPosX =
    MMExtremeRightPosX - cellSize_f * arrowsScale / 2.0f;
static constexpr float MMButtonsOffset =
    2 * cellSize_f * optionsScale;  // MM == Main Menu
static constexpr float MMOptionStartPosY =
    viewHeightPix_f * 0.4f;  // MM == Main Menu

class BoolOption : public sf::Drawable {
 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 protected:
  static constexpr float leftArrowPosX_m =
      rightArrowPosX - 5 * optionsScale * cellSize_f;
  static constexpr float centerPosX_m =
      (leftArrowPosX_m + rightArrowPosX) / 2.0f;
  bool& currState_m;
  float statePosY_m;
  Button leftArrow_m, rightArrow_m;
  MyText yes_m, no_m, name_m;

 public:
  BoolOption(Music& music, const std::string& name, float namePosY,
             bool& currState);
  void reloadAfterOpenScene(sf::Vector2f& mousePos, bool wasReleased);
  void handleEvents(sf::Event& event, sf::Vector2f& mousePos);
};