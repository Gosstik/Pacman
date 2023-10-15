#include "MyText.h"

bool MyText::imageLoaded_m = false;
sf::Image MyText::glyphsImg_m;
sf::Texture MyText::glyphsTex_m;
sf::Sprite MyText::glyphsSpr_m;

void MyText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(textSpr_m);
}

void MyText::constructStaticVariables() {
  if (!imageLoaded_m) {
    imageLoaded_m = true;
    glyphsImg_m.loadFromFile(imageFilename_m);
    glyphsImg_m.createMaskFromColor(
        sf::Color(0, 0, 0));  // remove black from image
    glyphsTex_m.loadFromImage(glyphsImg_m);
    glyphsSpr_m.setTexture(glyphsTex_m);
  }
}

MyText::MyText() { constructStaticVariables(); }

MyText::MyText(int value) {
  constructStaticVariables();
  setString(value);
}

MyText::MyText(const std::string& str) {
  constructStaticVariables();
  setString(str);
}

MyText::MyText(int value, float scaleX, float scaleY, float posX, float posY,
               bool origInCenter, const sf::Color& fillColor) {
  constructStaticVariables();
  setString(value, scaleX, scaleY, posX, posY, origInCenter, fillColor);
}

MyText::MyText(const std::string& str, float scaleX, float scaleY, float posX,
               float posY, bool origInCenter, const sf::Color& fillColor) {
  constructStaticVariables();
  setString(str, scaleX, scaleY, posX, posY, origInCenter, fillColor);
}

void MyText::setString(int value) {
  std::string str = std::to_string(value);
  setString(str);
}

void MyText::setString(int value, float scaleX, float scaleY, float posX,
                       float posY, bool origInCenter,
                       const sf::Color& fillColor) {
  std::string str = std::to_string(value);
  setString(str, scaleX, scaleY, posX, posY, origInCenter, fillColor);
}

void MyText::setString(const std::string& str) {
  if (str_m != str) {
    str_m = str;
    width_m = str_m.size() * (unsigned int)cellSize_i;
    height_m = (unsigned int)glyphSize_m;
    textTex_m.create(width_m, height_m);
    sf::RenderTexture textRenderTex_m;
    textRenderTex_m.create(width_m, height_m);
    textRenderTex_m.clear();

    for (size_t i = 0; i < str_m.size(); ++i) {
      currPosX_m = float(i) * cellSize_f;
      if (str[i] >= '0' && str_m[i] <= '9')
        glyphsSpr_m.setTextureRect(
            {(str_m[i] - '0') * cellSize_i, 0, glyphSize_m, glyphSize_m});
      else if (str_m[i] == ' ')
        setTexSpecialGlyph(spaceRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == '?')
        setTexSpecialGlyph(questRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == '!')
        setTexSpecialGlyph(exclRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == '/' || str_m[i] == '\\')
        setTexSpecialGlyph(slashRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == '-')
        setTexSpecialGlyph(dashRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == '"')
        setTexSpecialGlyph(dQuotRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == ':')
        setTexSpecialGlyph(colonRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == '<' || str_m[i] == '>')
        setTexSpecialGlyph(lAngleBracketRectLeft_m, specialGlyphsRectTop_m);
      else if (str_m[i] == '(' || str_m[i] == ')')
        setTexSpecialGlyph(lRoundBracket_m, specialGlyphsRectTop_m);
      else {
        glyphNum_m = str_m[i] - 'a';
        glyphRectLeft_m = (glyphNum_m % glyphsImgLen_m) * cellSize_i;
        glyphRectTop_m =
            (glyphNum_m / glyphsImgLen_m + 1) *
            cellSize_i;  // +1 needs because first line contains numbers
        glyphsSpr_m.setTextureRect(
            {glyphRectLeft_m, glyphRectTop_m, glyphSize_m, glyphSize_m});
      }
      glyphsSpr_m.setPosition(currPosX_m, 0);
      if (str_m[i] == '\\' || str_m[i] == '>' || str_m[i] == ')') {
        glyphsSpr_m.setOrigin(glyphSize_m / 2.0f, glyphSize_m / 2.0f);
        glyphsSpr_m.setPosition(currPosX_m + glyphSize_m / 1.5f,
                                glyphSize_m / 2.0f);  // todo: why 1.5 ???
        glyphsSpr_m.setRotation(180);
        textRenderTex_m.draw(glyphsSpr_m);
        glyphsSpr_m.setRotation(0);
        glyphsSpr_m.setOrigin(0, 0);
      } else {
        glyphsSpr_m.setPosition(currPosX_m, 0);
        textRenderTex_m.draw(glyphsSpr_m);
      }
    }

    textRenderTex_m.display();
    textTex_m = textRenderTex_m.getTexture();
    textImg_m = textTex_m.copyToImage();
    textImg_m.createMaskFromColor(sf::Color::Black);
    textTex_m.loadFromImage(textImg_m);
    textSpr_m.setTexture(textTex_m);
    textSpr_m.setTextureRect({0, 0, int(width_m), int(height_m)});

    textSpr_m.setColor(fillColor_m);
    if (origInCenter_m) setOriginInCenter();
  }
}

void MyText::setString(const std::string& str, float scaleX, float scaleY,
                       float posX, float posY, bool origInCenter,
                       const sf::Color& fillColor) {
  origInCenter_m = origInCenter;
  setString(str);
  setColor(fillColor);
  setPosition(posX, posY);
  setScale(scaleX, scaleY);
}

void MyText::setTexSpecialGlyph(const int& glyphRectLeft,
                                const int& glyphRectTop) {
  glyphsSpr_m.setTextureRect(
      {glyphRectLeft, glyphRectTop, glyphSize_m, glyphSize_m});
}

void MyText::setPosition(float x, float y) {
  posX_m = x;
  posY_m = y;
  textSpr_m.setPosition(posX_m, posY_m);
}

void MyText::setOrigin(float x, float y) {
  origInCenter_m = false;
  origX_m = x;
  origY_m = y;
  textSpr_m.setOrigin(origX_m, origY_m);
}

void MyText::setOriginInCenter() {
  origInCenter_m = true;
  origX_m = textSpr_m.getLocalBounds().width / 2.0f;
  origY_m = textSpr_m.getLocalBounds().height / 2.0f;
  textSpr_m.setOrigin(origX_m, origY_m);
}

void MyText::setScale(float scaleX, float scaleY) {
  scaleX_m = scaleX;
  scaleY_m = scaleY;
  textSpr_m.setScale(scaleX_m, scaleY_m);
}

void MyText::setColor(const sf::Color& color) {
  fillColor_m = color;
  textSpr_m.setColor(fillColor_m);
}

sf::FloatRect MyText::getLocalBounds() const {
  return textSpr_m.getLocalBounds();
}

sf::FloatRect MyText::getGlobalBounds() const {
  return textSpr_m.getGlobalBounds();
}

const sf::Vector2f& MyText::getScale() const { return textSpr_m.getScale(); }

const std::string& MyText::getString() const { return str_m; }

sf::Vector2f MyText::getPosition() const { return textSpr_m.getPosition(); }