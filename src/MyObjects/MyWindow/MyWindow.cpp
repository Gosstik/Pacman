#include "MyWindow.h"

//#define sc(x, y) static_cast<x>(y)

MyWindow::MyWindow() {
  //    auto x = sc(int, 1.0);
  vm_m = sf::VideoMode::getDesktopMode();
  size_t minValue = std::min(vm_m.width, vm_m.height);
  if (minValue < viewHeightPix_i) {
    std::cerr << "Your resolution " << vm_m.width << "*" << vm_m.height
              << " is not supported.\n";
    exit(99);
  }
  float coef = float(minValue) * 0.9f / viewHeightPix_f;
  float windowWidthPix_f = viewWidthPix_f * coef;  // todo redo
  float windowHeightPix_f = windowWidthPix_f * viewHeightPix_f / viewWidthPix_f;
  int windowWidthPix_i = int(windowWidthPix_f);
  int windowHeightPix_i = int(windowHeightPix_f);
  create(sf::VideoMode(windowWidthPix_i, windowHeightPix_i), "Pacman",
         sf::Style::Titlebar | sf::Style::Close);
  sf::View view({0, 0, viewWidthPix_f, viewHeightPix_f});
  setView(view);
  setPosition(sf::Vector2i(int(vm_m.width) / 2 - windowWidthPix_i / 2,
                           int(vm_m.height) / 2 - windowHeightPix_i / 2));

  //    setFramerateLimit(1000);
  //    setVerticalSyncEnabled(false);

  screenshotTex_m.create(windowWidthPix_i, windowHeightPix_i);
}

void MyWindow::makeScreenshot() { screenshotTex_m = rendTex.getTexture(); }

sf::Texture MyWindow::getScreenshotTex() { return screenshotTex_m; }