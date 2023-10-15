#pragma once

#include "Globals.h"

class MyWindow : public sf::RenderWindow {
  sf::VideoMode vm_m;
  sf::Texture screenshotTex_m;

 public:
  sf::RenderTexture rendTex;
  using sf::RenderWindow::draw;

  MyWindow();
  void makeScreenshot();
  sf::Texture getScreenshotTex();
};