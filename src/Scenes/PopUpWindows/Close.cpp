#include "Close.h"

Close::Close(Music& music)
    : setPopUpWindowSoundRef_m(music.setPopUpWindowSound),
      quest_m("?", questScale_m, questScale_m, questPosX_m, questPosY_m, true),
      text1_m("exit game?", text1Scale_m, text1Scale_m, text1PosX_m,
              text1PosY_m, false),
      text2_m("unsaved data will", text2Scale_m, text2Scale_m, text2PosX_m,
              text2PosY_m, false, text2Color_m),
      text3_m("be lost", text3Scale_m, text3Scale_m, text3PosX_m, text3PosY_m,
              false, text3Color_m),
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

  // creating window-close
  rendTex.draw(quest_m);
  rendTex.draw(text1_m);
  rendTex.draw(text2_m);
  rendTex.draw(text3_m);
  rendTex.display();
  windowTex_m = rendTex.getTexture();
  windowSpr_m.setTexture(windowTex_m);
  windowSpr_m.setOrigin(windowSpr_m.getLocalBounds().width / 2.0f,
                        windowSpr_m.getLocalBounds().height / 2.0f);
  windowSpr_m.setPosition(windowPosX_m, windowPosY_m);

  screenshotTex_m.create(viewWidthPix_i, viewHeightPix_i);
}

void Close::open(MyWindow& window,
                 std::stack<std::reference_wrapper<bool>>& prevScene,
                 TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_Close) {
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

void Close::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  if (!calledByExitButton) setPopUpWindowSoundRef_m.play();
  calledByExitButton = false;

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

void Close::handleEvents(MyWindow& window,
                         std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::MouseMoved) {
      mousePos_m = {float(event_m.mouseMove.x), float(event_m.mouseMove.y)};
      mousePos_m =
          window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
    }
    yes_m.update(event_m, mousePos_m);
    no_m.update(event_m, mousePos_m);
    if (yes_m.wasActivated()) {
      yes_m.reload();
      event_FullClose = true;
      event_Close = false;
    } else if (no_m.wasActivated()) {
      no_m.reload();
      prevScene.top().get() = true;
      prevScene.pop();
      event_Close = false;
    }
  }
}

template <class T>
void Close::displayObjects(T& window, TechInfo& techInfo, bool makeScreenshot) {
  window.clear(sf::Color::Black);

  window.draw(screenshotSpr_m);
  window.draw(windowSpr_m);
  window.draw(yes_m);
  window.draw(no_m);
  if (!makeScreenshot) window.draw(techInfo);

  window.display();
}
template void Close::displayObjects<MyWindow>(MyWindow& window,
                                              TechInfo& techInfo,
                                              bool makeScreenshot);
template void Close::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void Close::makeScreenshot(MyWindow& window, TechInfo& techInfo) {
  window.rendTex.create(viewWidthPix_i, viewHeightPix_i);
  screenshotSpr_m.setColor(sf::Color(255, 255, 255));
  displayObjects(window.rendTex, techInfo, true);
  window.makeScreenshot();
}