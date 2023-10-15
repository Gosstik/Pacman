#pragma once

#include <string>

#include "Globals.h"

class MyText : public sf::Drawable {
  static const std::string imageFilename_m;

  static constexpr int glyphSize_m = cellSize_i - cellSize_i / 8;
  static constexpr int glyphsImgLen_m =
      10;  // number of symbols in one string in "glyphs.png"
  static constexpr int specialGlyphsRectTop_m = 4 * cellSize_i;
  static constexpr int spaceRectLeft_m = 0;               // space
  static constexpr int questRectLeft_m = 1 * cellSize_i;  // question mark
  static constexpr int exclRectLeft_m = 2 * cellSize_i;   // exclamation mark
  static constexpr int slashRectLeft_m = 3 * cellSize_i;  // slash
  static constexpr int dashRectLeft_m = 4 * cellSize_i;   // dash
  static constexpr int dQuotRectLeft_m =
      5 * cellSize_i;  // double quotation mark
  static constexpr int colonRectLeft_m = 6 * cellSize_i;  // colon
  static constexpr int lAngleBracketRectLeft_m =
      7 * cellSize_i;  // sprLeft angle bracket
  static constexpr int lRoundBracket_m =
      8 * cellSize_i;  // sprLeft round bracket

  static sf::Image glyphsImg_m;
  static sf::Texture glyphsTex_m;
  static sf::Sprite glyphsSpr_m;
  static bool imageLoaded_m;

  sf::Texture currGlyph_m;

  // data variables
  sf::Image textImg_m;
  sf::Texture textTex_m;
  sf::Sprite textSpr_m;
  std::string str_m;
  size_t width_m = 0, height_m = 0;
  float posX_m = 0, posY_m = 0;
  float origX_m = 0, origY_m = 0;
  float scaleX_m = 1.0f, scaleY_m = 1.0f;
  bool origInCenter_m = false;
  sf::Color fillColor_m = sf::Color::White;

  // temporary variables
  int glyphNum_m = 0;
  int glyphRectLeft_m = 0;
  int glyphRectTop_m = 0;
  float currPosX_m = 0;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  static void constructStaticVariables();
  static void setTexSpecialGlyph(const int& glyphRectLeft,
                                 const int& glyphRectTop);

 public:
  MyText();
  explicit MyText(int value);
  MyText(int value, float scaleX, float scaleY, float posX, float posY,
         bool origInCenter, const sf::Color& fillColor = sf::Color::White);
  explicit MyText(const std::string& str);
  MyText(const std::string& str, float scaleX, float scaleY, float posX,
         float posY, bool origInCenter,
         const sf::Color& fillColor = sf::Color::White);

  void setString(int value);
  void setString(int value, float scaleX, float scaleY, float posX, float posY,
                 bool origInCenter, const sf::Color& fillColor);
  void setString(const std::string& str);
  void setString(const std::string& str, float scaleX, float scaleY, float posX,
                 float posY, bool origInCenter, const sf::Color& fillColor);

  void setPosition(float x, float y);
  void setOrigin(float x, float y);
  void setOriginInCenter();
  void setScale(float scaleX, float scaleY);
  void setColor(const sf::Color& color);
  sf::FloatRect getLocalBounds() const;
  sf::FloatRect getGlobalBounds() const;
  const sf::Vector2f& getScale() const;
  const std::string& getString() const;
  sf::Vector2f getPosition() const;
};