#include "MainMenuSettings.h"

MainMenuSettings::MainMenuSettings(Music& music)
    : backSoundRef_m(music.backSound),
      settings_m("settings", settingsScale_m, settingsScale_m, settingsPosX_m,
                 settingsPosY_m, true),
      gameButton_m(music, "game", optionsScale, gamePosX_m, headersPosY_m,
                   true),
      musicButton_m(music, "music", optionsScale, musicPosX, headersPosY_m,
                    true),
      graphicsButton_m(music, "graphics", optionsScale, graphicsPosX,
                       headersPosY_m, true),
      devButton_m(music, "dev", optionsScale, devPosX, headersPosY_m, true),
      gameOptions_m(music),
      musicOptions_m(music),
      devOptions_m(music),
      graphicsOptions_m(music) {
  gameButton_m.activate();
  if (!gameFirstOpen) readData();
}

MainMenuSettings::~MainMenuSettings() { saveData(); }

void MainMenuSettings::saveData() {
  gameOptions_m.saveData();
  musicOptions_m.saveData();
  graphicsOptions_m.saveData();
  devOptions_m.saveData();
}

void MainMenuSettings::readData() {
  gameOptions_m.readData();
  musicOptions_m.readData();
  graphicsOptions_m.readData();
  devOptions_m.readData();
}

void MainMenuSettings::open(MyWindow& window,
                            std::stack<std::reference_wrapper<bool>>& prevScene,
                            TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_MainMenuSettings) {
    techInfo.handleTime();
    while (techInfo.runCycle()) {
      techInfo.reloadRunCycle();
      handleEvents(window, prevScene);
      if (!event_MainMenuSettings) break;

      dTime_m = techInfo.getCycleDuration();
    }
    if (techInfo.runDisplay()) {
      techInfo.reloadRunDisplay();
      displayObjects(window, techInfo, false);
    }
  }

  if (event_Close) {
    cameFromCloseWindow_m = true;
    makeScreenshot(window, techInfo);
  } else
    calledFromPause = false;
}

void MainMenuSettings::handleEvents(
    MyWindow& window, std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::MouseMoved) {
      mousePos_m = {float(event_m.mouseMove.x), float(event_m.mouseMove.y)};
      mousePos_m =
          window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
    }
    gameButton_m.update(event_m, mousePos_m);
    musicButton_m.update(event_m, mousePos_m);
    graphicsButton_m.update(event_m, mousePos_m);
    devButton_m.update(event_m, mousePos_m);

    if (currHeader_m == SettingsHeader::game)
      gameOptions_m.handleEvents(event_m, mousePos_m);
    else if (currHeader_m == SettingsHeader::music)
      musicOptions_m.handleEvents(event_m, mousePos_m);
    else if (currHeader_m == SettingsHeader::graphics)
      graphicsOptions_m.handleEvents(event_m, mousePos_m);
    else
      devOptions_m.handleEvents(event_m, mousePos_m);

    if (event_m.type == sf::Event::Closed) {
      event_Close = true;
      event_MainMenuSettings = false;
      prevScene.push(event_MainMenuSettings);
    } else if (event_m.type == sf::Event::KeyPressed &&
               event_m.key.code == sf::Keyboard::Escape) {
      backSoundRef_m.play();
      prevScene.top().get() = true;
      prevScene.pop();
      event_MainMenuSettings = false;
    } else if (gameButton_m.wasActivated()) {
      gameButton_m.reload();
      deactivateCurrOptionButton();
      currHeader_m = SettingsHeader::game;
    } else if (musicButton_m.wasActivated()) {
      musicButton_m.reload();
      deactivateCurrOptionButton();
      currHeader_m = SettingsHeader::music;
    } else if (graphicsButton_m.wasActivated()) {
      graphicsButton_m.reload();
      deactivateCurrOptionButton();
      currHeader_m = SettingsHeader::graphics;
    } else if (devButton_m.wasActivated()) {
      devButton_m.reload();
      deactivateCurrOptionButton();
      currHeader_m = SettingsHeader::dev;
    }
  }
}

void MainMenuSettings::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  mousePos_m = {float(sf::Mouse::getPosition(window).x),
                float(sf::Mouse::getPosition(window).y)};
  mousePos_m = window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
  bool wasReleased;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    wasReleased = false;
  else
    wasReleased = true;
  if (cameFromCloseWindow_m) {
    cameFromCloseWindow_m = false;
    if (!gameButton_m.isActivated())
      gameButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);
    if (!musicButton_m.isActivated())
      musicButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);
    if (!graphicsButton_m.isActivated())
      graphicsButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);
    if (!devButton_m.isActivated())
      devButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  } else {
    gameButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);
    musicButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);
    graphicsButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);
    devButton_m.reloadAfterOpenScene(mousePos_m, wasReleased);

    currHeader_m = SettingsHeader::game;
    gameButton_m.activate();
  }

  gameOptions_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  musicOptions_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  graphicsOptions_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  devOptions_m.reloadAfterOpenScene(mousePos_m, wasReleased);

  dTime_m = techInfo.getCycleDuration();
}

template <class T>
void MainMenuSettings::displayObjects(T& window, TechInfo& techInfo,
                                      bool makeScreenshot) {
  window.clear();

  window.draw(settings_m);
  window.draw(gameButton_m);
  window.draw(musicButton_m);
  window.draw(graphicsButton_m);
  window.draw(devButton_m);

  if (currHeader_m == SettingsHeader::game)
    window.draw(gameOptions_m);
  else if (currHeader_m == SettingsHeader::music)
    window.draw(musicOptions_m);
  else if (currHeader_m == SettingsHeader::graphics)
    window.draw(graphicsOptions_m);
  else
    window.draw(devOptions_m);

  if (!makeScreenshot) window.draw(techInfo);

  window.display();
}
template void MainMenuSettings::displayObjects<MyWindow>(MyWindow& window,
                                                         TechInfo& techInfo,
                                                         bool makeScreenshot);
template void MainMenuSettings::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void MainMenuSettings::makeScreenshot(MyWindow& window, TechInfo& techInfo) {
  window.rendTex.create(viewWidthPix_i, viewHeightPix_i);
  displayObjects(window.rendTex, techInfo, true);
  window.makeScreenshot();
}

void MainMenuSettings::deactivateCurrOptionButton() {
  if (currHeader_m == SettingsHeader::game)
    gameButton_m.deactivate();
  else if (currHeader_m == SettingsHeader::music)
    musicButton_m.deactivate();
  else if (currHeader_m == SettingsHeader::graphics)
    graphicsButton_m.deactivate();
  else
    devButton_m.deactivate();
}