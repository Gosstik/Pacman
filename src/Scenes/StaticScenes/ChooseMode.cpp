#include "ChooseMode.h"

ChooseMode::ChooseMode(Music& music)
    : chooseMode_m("choose mode", chooseModeScale_m, chooseModeScale_m,
                   chooseModePosX_m, chooseModePosY_m, true),
      classicGame_m(music, "classic game", buttonsScale_m, classicGamePosX_m,
                    classicGamePosY_m),
      endlessMap_m(music, "endless map", buttonsScale_m, endlessMapPosX_m,
                   endlessMapPosY_m),
      oneUp_m(music, "1up", buttonsScale_m, oneUpPosX_m, oneUpPosY_m),
      twoUp_m(music, "2up", buttonsScale_m, twoUpPosX_m, twoUpPosY_m),
      network_m(music, "network", buttonsScale_m, networkPosX_m, networkPosY_m),
      backSoundRef_m(music.backSound) {
  if (currMode.mode == CurrMode::classicGame)
    classicGame_m.activate();
  else
    endlessMap_m.activate();

  if (currMode.up == CurrMode::one)
    oneUp_m.activate();
  else if (currMode.up == CurrMode::two)
    twoUp_m.activate();
  else
    network_m.activate();
}

void ChooseMode::open(MyWindow& window,
                      std::stack<std::reference_wrapper<bool>>& prevScene,
                      TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_ChooseMode) {
    techInfo.handleTime();
    while (techInfo.runCycle()) {
      techInfo.reloadRunCycle();
      handleEvents(window, prevScene);
      if (!event_ChooseMode) break;

      comingSoon_m.update(dTime_m);
    }
    if (techInfo.runDisplay()) {
      techInfo.reloadRunDisplay();
      displayObjects(window, techInfo, false);
    }
  }

  if (event_Close) makeScreenshot(window, techInfo);

  comingSoon_m.show(false);
}

void ChooseMode::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  mousePos_m = {float(sf::Mouse::getPosition(window).x),
                float(sf::Mouse::getPosition(window).y)};
  mousePos_m = window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
  bool wasReleased;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    wasReleased = false;
  else
    wasReleased = true;

  classicGame_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  endlessMap_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  oneUp_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  twoUp_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  network_m.reloadAfterOpenScene(mousePos_m, wasReleased);

  dTime_m = techInfo.getCycleDuration();
}

void ChooseMode::handleEvents(
    MyWindow& window, std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::MouseMoved) {
      mousePos_m = {float(event_m.mouseMove.x), float(event_m.mouseMove.y)};
      mousePos_m =
          window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
    }
    classicGame_m.update(event_m, mousePos_m);
    endlessMap_m.update(event_m, mousePos_m);
    oneUp_m.update(event_m, mousePos_m);
    twoUp_m.update(event_m, mousePos_m);
    network_m.update(event_m, mousePos_m);
    if (event_m.type == sf::Event::Closed) {
      event_Close = true;
      event_ChooseMode = false;
      prevScene.push(event_ChooseMode);
    } else if (event_m.type == sf::Event::KeyPressed &&
               event_m.key.code == sf::Keyboard::Escape) {
      backSoundRef_m.play();
      event_MainMenu = true;
      event_ChooseMode = false;
    } else if (classicGame_m.wasActivated()) {
      classicGame_m.reload();
      if (currMode.mode != CurrMode::Mode::classicGame) {
        deactivateCurrMode();
        currMode.mode = CurrMode::Mode::classicGame;
      }
    } else if (endlessMap_m.wasActivated()) {
      endlessMap_m.reload();
      endlessMap_m.deactivate(mousePos_m);
      comingSoon_m.show(true);
      //            if (currMode.mode != CurrMode::Mode::endlessMap) {
      //                deactivateCurrMode();
      //                currMode.mode = CurrMode::Mode::endlessMap;
      //            }
    } else if (oneUp_m.wasActivated()) {
      oneUp_m.reload();
      if (currMode.up != CurrMode::Up::one) {
        deactivateCurrUp();
        currMode.up = CurrMode::Up::one;
      }
    } else if (twoUp_m.wasActivated()) {
      twoUp_m.reload();
      twoUp_m.deactivate(mousePos_m);
      comingSoon_m.show(true);
      //            if (currMode.up != CurrMode::Up::two) {
      //                deactivateCurrUp();
      //                currMode.up = CurrMode::Up::two;
      //            }
    } else if (network_m.wasActivated()) {
      network_m.reload();
      network_m.deactivate(mousePos_m);
      comingSoon_m.show(true);
      //            if (currMode.up != CurrMode::Up::network) {
      //                deactivateCurrUp();
      //                currMode.up = CurrMode::Up::network;
      //            }
    }
  }
}

template <class T>
void ChooseMode::displayObjects(T& window, TechInfo& techInfo,
                                bool makeScreenshot) {
  window.clear();

  window.draw(pacmanTitle_m);
  window.draw(chooseMode_m);
  window.draw(classicGame_m);
  window.draw(endlessMap_m);
  window.draw(oneUp_m);
  window.draw(twoUp_m);
  window.draw(network_m);
  window.draw(comingSoon_m);
  if (!makeScreenshot) window.draw(techInfo);

  window.display();
}
template void ChooseMode::displayObjects<MyWindow>(MyWindow& window,
                                                   TechInfo& techInfo,
                                                   bool makeScreenshot);
template void ChooseMode::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void ChooseMode::makeScreenshot(MyWindow& window, TechInfo& techInfo) {
  window.rendTex.create(viewWidthPix_i, viewHeightPix_i);
  displayObjects(window.rendTex, techInfo, true);
  window.makeScreenshot();
}

void ChooseMode::deactivateCurrMode() {
  if (currMode.mode == CurrMode::Mode::classicGame)
    classicGame_m.deactivate(mousePos_m);
  else
    endlessMap_m.deactivate(mousePos_m);
}

void ChooseMode::deactivateCurrUp() {
  if (currMode.up == CurrMode::Up::one)
    oneUp_m.deactivate(mousePos_m);
  else if (currMode.up == CurrMode::Up::two)
    twoUp_m.deactivate(mousePos_m);
  else
    network_m.deactivate(mousePos_m);
}