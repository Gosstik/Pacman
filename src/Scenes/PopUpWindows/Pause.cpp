#include "Pause.h"

Pause::Pause(Music& music)
    : backSoundRef_m(music.backSound),
      setPopUpWindowSoundRef_m(music.setPopUpWindowSound),
      pause_m("pause", pauseScale_m, pauseScale_m, pausePosX_m, pausePosY_m,
              true),
      resume_m(music, "resume", resumeScale_m, resumePosX_m, resumePosY_m),
      restart_m(music, "restart", restartScale_m, restartPosX_m, restartPosY_m),
      settings_m(music, "settings", settingsScale_m, settingsPosX_m,
                 settingsPosY_m),
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

  // creating window-pause
  rendTex.draw(pause_m);
  rendTex.display();
  windowTex_m = rendTex.getTexture();
  windowSpr_m.setTexture(windowTex_m);
  windowSpr_m.setOrigin(windowSpr_m.getLocalBounds().width / 2.0f,
                        windowSpr_m.getLocalBounds().height / 2.0f);
  windowSpr_m.setPosition(windowPosX_m, windowPosY_m);

  screenshotTex_m.create(viewWidthPix_i, viewHeightPix_i);
}

void Pause::open(MyWindow& window,
                 std::stack<std::reference_wrapper<bool>>& prevScene,
                 TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_Pause) {
    techInfo.handleTime();
    while (techInfo.runCycle()) {
      techInfo.reloadRunCycle();
      handleEvents(window, prevScene);
      if (!event_Pause) break;
    }
    if (techInfo.runDisplay()) {
      techInfo.reloadRunDisplay();
      displayObjects(window, techInfo, false);
    }
  }

  if (resume_m.wasActivated())
    calledFromGameMode = true;
  else if (event_Close || event_PauseSettings || event_BackMainMenu)
    makeScreenshot(window, techInfo);
}

void Pause::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  if (calledFromGameMode) {
    setPopUpWindowSoundRef_m.play();
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
  resume_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  restart_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  settings_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  mainMenu_m.reloadAfterOpenScene(mousePos_m, wasReleased);

  dTime_m = techInfo.getCycleDuration();
}

void Pause::handleEvents(MyWindow& window,
                         std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::MouseMoved) {
      mousePos_m = {float(event_m.mouseMove.x), float(event_m.mouseMove.y)};
      mousePos_m =
          window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
    }
    resume_m.update(event_m, mousePos_m);
    restart_m.update(event_m, mousePos_m);
    settings_m.update(event_m, mousePos_m);
    mainMenu_m.update(event_m, mousePos_m);
    if (event_m.type == sf::Event::Closed) {
      event_Close = true;
      prevScene.push(event_Pause);
      event_Pause = false;
    } else if (event_m.type == sf::Event::KeyPressed &&
               event_m.key.code == sf::Keyboard::Escape) {
      backSoundRef_m.play();
      prevScene.top().get() = true;
      prevScene.pop();
      event_Pause = false;
    } else if (resume_m.wasActivated()) {
      resume_m.reload();
      prevScene.top().get() = true;
      prevScene.pop();
      event_Pause = false;
    } else if (restart_m.wasActivated()) {
      restart_m.reload();
      event_NewGame = true;
      prevScene.top().get() = true;
      prevScene.pop();
      event_Pause = false;
    } else if (settings_m.wasActivated()) {
      settings_m.reload();
      event_MainMenuSettings = true;
      calledFromPause = true;
      prevScene.push(event_Pause);
      event_Pause = false;
    } else if (mainMenu_m.wasActivated()) {
      mainMenu_m.reload();
      event_BackMainMenu = true;
      prevScene.push(event_Pause);
      event_Pause = false;
    }
  }
}

template <class T>
void Pause::displayObjects(T& window, TechInfo& techInfo, bool makeScreenshot) {
  window.clear();

  window.draw(screenshotSpr_m);
  window.draw(windowSpr_m);
  window.draw(resume_m);
  window.draw(restart_m);
  window.draw(settings_m);
  window.draw(mainMenu_m);
  if (!makeScreenshot) window.draw(techInfo);

  window.display();
}
template void Pause::displayObjects<MyWindow>(MyWindow& window,
                                              TechInfo& techInfo,
                                              bool makeScreenshot);
template void Pause::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void Pause::makeScreenshot(MyWindow& window, TechInfo& techInfo) {
  window.rendTex.create(viewWidthPix_i, viewHeightPix_i);
  screenshotSpr_m.setColor(sf::Color(255, 255, 255));
  displayObjects(window.rendTex, techInfo, true);
  window.makeScreenshot();
}