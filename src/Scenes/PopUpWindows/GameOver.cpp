#include "GameOver.h"

GameOver::GameOver(Music& music)
    : gameOver_m("game over", gameOverScale_m, gameOverScale_m, gameOverPosX_m,
                 gameOverPosY_m, true),
      restart_m(music, "restart", restartScale_m, restartPosX_m, restartPosY_m),
      mainMenu_m(music, "main menu", mainMenuScale_m, mainMenuPosX_m,
                 mainMenuPosY_m) {
  sf::RenderTexture rendTex;
  rendTex.create((unsigned int)windowWidth_m, (unsigned int)windowHeight_m);
  rendTex.clear();

  sf::RectangleShape topRect(sf::Vector2f(float(rendTex.getSize().x),
                                          float(rendTex.getSize().y) * 0.3f));
  topRect.setFillColor(sf::Color(60, 60, 60));
  rendTex.draw(topRect);

  sf::RectangleShape bottomRect(
      sf::Vector2f(float(rendTex.getSize().x),
                   float(rendTex.getSize().y) - topRect.getSize().y));
  bottomRect.setFillColor(sf::Color(40, 40, 40));
  bottomRect.setPosition(0, topRect.getSize().y);
  rendTex.draw(bottomRect);

  sf::RectangleShape transparentRect(sf::Vector2f(
      float(rendTex.getSize().x) - 2.0f, float(rendTex.getSize().y) - 2.0f));
  transparentRect.setFillColor(sf::Color::Transparent);
  transparentRect.setOutlineThickness(1);
  transparentRect.setOutlineColor(sf::Color::White);
  transparentRect.setPosition(1.0f, 1.0f);
  rendTex.draw(transparentRect);

  // creating window-gameOver
  rendTex.draw(gameOver_m);
  rendTex.display();
  windowTex_m = rendTex.getTexture();
  windowSpr_m.setTexture(windowTex_m);
  windowSpr_m.setOrigin(windowSpr_m.getLocalBounds().width / 2.0f,
                        windowSpr_m.getLocalBounds().height / 2.0f);
  windowSpr_m.setPosition(windowPosX_m, windowPosY_m);

  screenshotTex_m.create(viewWidthPix_i, viewHeightPix_i);
}

void GameOver::open(MyWindow& window,
                    std::stack<std::reference_wrapper<bool>>& prevScene,
                    TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_GameOver) {
    techInfo.handleTime();
    while (techInfo.runCycle()) {
      techInfo.reloadRunCycle();
      handleEvents(window, prevScene);
      if (!event_GameOver) break;
    }
    if (techInfo.runDisplay()) {
      techInfo.reloadRunDisplay();
      displayObjects(window, techInfo, false);
    }
  }

  if (event_Close) makeScreenshot(window, techInfo);
}

void GameOver::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  if (calledFromGameMode) {
    calledFromGameMode = false;
    screenshotTex_m = window.getScreenshotTex();
    screenshotSpr_m.setTexture(screenshotTex_m);
  }
  screenshotSpr_m.setColor(darkeningColor);

  mousePos_m = {float(sf::Mouse::getPosition(window).x),
                float(sf::Mouse::getPosition(window).y)};
  mousePos_m = window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
  bool wasReleased;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    wasReleased = false;
  else
    wasReleased = true;
  restart_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  mainMenu_m.reloadAfterOpenScene(mousePos_m, wasReleased);

  dTime_m = techInfo.getCycleDuration();
}

void GameOver::handleEvents(
    MyWindow& window, std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::MouseMoved) {
      mousePos_m = {float(event_m.mouseMove.x), float(event_m.mouseMove.y)};
      mousePos_m =
          window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
    }
    restart_m.update(event_m, mousePos_m);
    mainMenu_m.update(event_m, mousePos_m);
    if (event_m.type == sf::Event::Closed) {
      event_Close = true;
      prevScene.push(event_GameOver);
      event_GameOver = false;
    } else if (restart_m.wasActivated()) {
      restart_m.reload();
      event_NewGame = true;
      prevScene.top().get() = true;
      prevScene.pop();
      event_GameOver = false;
    } else if (mainMenu_m.wasActivated()) {
      mainMenu_m.reload();
      event_NewGame = true;  // to start game from the beginning entering from
                             // main menu next time
      event_MainMenu = true;
      while (!prevScene.empty()) prevScene.pop();
      event_GameOver = false;
    }
  }
}

template <class T>
void GameOver::displayObjects(T& window, TechInfo& techInfo,
                              bool makeScreenshot) {
  window.clear();

  window.draw(screenshotSpr_m);
  window.draw(windowSpr_m);
  window.draw(restart_m);
  window.draw(mainMenu_m);
  if (!makeScreenshot) window.draw(techInfo);

  window.display();
}
template void GameOver::displayObjects<MyWindow>(MyWindow& window,
                                                 TechInfo& techInfo,
                                                 bool makeScreenshot);
template void GameOver::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void GameOver::makeScreenshot(MyWindow& window, TechInfo& techInfo) {
  window.rendTex.create(viewWidthPix_i, viewHeightPix_i);
  screenshotSpr_m.setColor(sf::Color(255, 255, 255));
  displayObjects(window.rendTex, techInfo, true);
  window.makeScreenshot();
}