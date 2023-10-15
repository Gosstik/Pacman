#include "BackMainMenu.h"

BackMainMenu::BackMainMenu(Music& music)
    : backSoundRef_m(music.backSound),
      quest_m("?", questScale_m, questScale_m, questPosX_m, questPosY_m, true),
      text1_m("back to main", text1Scale_m, text1Scale_m, text1PosX_m,
              text1PosY_m, false),
      text2_m("menu?", text2Scale_m, text2Scale_m, text2PosX_m, text2PosY_m,
              false),
      text3_m("unsaved data will", text3Scale_m, text3Scale_m, text3PosX_m,
              text3PosY_m, false, text3Color_m),
      text4_m("be lost", text4Scale_m, text4Scale_m, text4PosX_m, text4PosY_m,
              false, text4Color_m),
      yes_m(music, "yes", yesScale_m, yesPosX_m, yesPosY_m),
      no_m(music, "no", noScale_m, noPosX_m, noPosY_m) {
  sf::RenderTexture rendTex;
  rendTex.create((unsigned int)windowWidth_m, (unsigned int)windowHeight_m);
  rendTex.clear();
  sf::RectangleShape rect(sf::Vector2f(float(rendTex.getSize().x) - 2.0f,
                                       float(rendTex.getSize().y) - 2.0f));
  rect.setFillColor(sf::Color(40, 40, 40));
  rect.setOutlineThickness(1);
  rect.setOutlineColor(sf::Color::White);
  rect.setPosition(1.0f, 1.0f);
  rendTex.draw(rect);

  // create window-backMainMenu
  rendTex.draw(quest_m);
  rendTex.draw(text1_m);
  rendTex.draw(text2_m);
  rendTex.draw(text3_m);
  rendTex.draw(text4_m);
  rendTex.display();
  windowTex_m = rendTex.getTexture();
  windowSpr_m.setTexture(windowTex_m);
  windowSpr_m.setOrigin(windowSpr_m.getLocalBounds().width / 2.0f,
                        windowSpr_m.getLocalBounds().height / 2.0f);
  windowSpr_m.setPosition(windowPosX_m, windowPosY_m);

  screenshotTex_m.create(viewWidthPix_i, viewHeightPix_i);
}

void BackMainMenu::open(MyWindow& window,
                        std::stack<std::reference_wrapper<bool>>& prevScene,
                        TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_BackMainMenu) {
    techInfo.handleTime();
    while (techInfo.runCycle()) {
      techInfo.reloadRunCycle();
      handleEvents(window, prevScene);
    }
    if (techInfo.runDisplay()) {
      techInfo.reloadRunDisplay();
      displayObjects(window, techInfo);
    }
  }
}

void BackMainMenu::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  screenshotTex_m = window.getScreenshotTex();
  screenshotSpr_m.setTexture(screenshotTex_m);
  screenshotSpr_m.setColor(darkeningColor);

  mousePos_m = {float(sf::Mouse::getPosition(window).x),
                float(sf::Mouse::getPosition(window).y)};
  mousePos_m = window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
  bool wasReleased;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    wasReleased = false;
  else
    wasReleased = true;
  yes_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  no_m.reloadAfterOpenScene(mousePos_m, wasReleased);
}

void BackMainMenu::handleEvents(
    MyWindow& window, std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::MouseMoved) {
      mousePos_m = {float(event_m.mouseMove.x), float(event_m.mouseMove.y)};
      mousePos_m =
          window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
    }
    yes_m.update(event_m, mousePos_m);
    no_m.update(event_m, mousePos_m);
    if (event_m.type == sf::Event::Closed) {
      event_Close = true;
      event_BackMainMenu = false;
      prevScene.push(event_BackMainMenu);
    }
    if (yes_m.wasActivated()) {
      yes_m.reload();
      event_MainMenu = true;
      while (!prevScene.empty()) prevScene.pop();
      event_BackMainMenu = false;
    } else if (no_m.wasActivated()) {
      no_m.reload();
      prevScene.top().get() = true;
      prevScene.pop();
      event_BackMainMenu = false;
    } else if (event_m.type == sf::Event::KeyPressed &&
               event_m.key.code == sf::Keyboard::Escape) {
      backSoundRef_m.play();
      prevScene.top().get() = true;
      prevScene.pop();
      event_BackMainMenu = false;
    }
  }
}

template <class T>
void BackMainMenu::displayObjects(T& window, TechInfo& techInfo,
                                  bool makeScreenshot) {
  window.clear();

  window.draw(screenshotSpr_m);
  window.draw(windowSpr_m);
  window.draw(yes_m);
  window.draw(no_m);
  if (!makeScreenshot) window.draw(techInfo);

  window.display();
}
template void BackMainMenu::displayObjects<MyWindow>(MyWindow& window,
                                                     TechInfo& techInfo,
                                                     bool makeScreenshot);
template void BackMainMenu::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void BackMainMenu::makeScreenshot(MyWindow& window, TechInfo& techInfo) {}