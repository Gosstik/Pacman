#include "MainMenu.h"

MainMenu::MainMenu(Music& music)
    : mainMenu_m("main menu", mainMenuScale_m, mainMenuScale_m, mainMenuPosX_m,
                 mainMenuPosY_m, true),
      MMSoundtrackRef_m(music.MMSoundtrack),
      startGame_m(music, "start game", buttonsScale_m, startGamePosX_m,
                  startGamePosY_m),
      chooseMode_m(music, "choose mode", buttonsScale_m, chooseModePosX_m,
                   chooseModePosY_m),
      profiles_m(music, "profiles", buttonsScale_m, profilesPosX_m,
                 profilesPosY_m),
      settings_m(music, "settings", buttonsScale_m, settingsPosX_m,
                 settingsPosY_m),
      exit_m(music, "exit", buttonsScale_m, exitPosX_m, exitPosY_m) {}

void MainMenu::open(MyWindow& window,
                    std::stack<std::reference_wrapper<bool>>& prevScene,
                    TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_MainMenu) {
    techInfo.handleTime();
    while (techInfo.runCycle()) {
      techInfo.reloadRunCycle();
      handleEvents(window, prevScene);
      if (!event_MainMenu) break;

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

void MainMenu::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  if (playMusicInMM_b)
    if (MMSoundtrackRef_m.getStatus() != sf::SoundSource::Playing)
      MMSoundtrackRef_m.play();

  mousePos_m = {float(sf::Mouse::getPosition(window).x),
                float(sf::Mouse::getPosition(window).y)};
  mousePos_m = window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
  bool wasReleased;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    wasReleased = false;
  else
    wasReleased = true;
  startGame_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  chooseMode_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  profiles_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  settings_m.reloadAfterOpenScene(mousePos_m, wasReleased);
  exit_m.reloadAfterOpenScene(mousePos_m, wasReleased);

  dTime_m = techInfo.getCycleDuration();
}

void MainMenu::handleEvents(
    MyWindow& window, std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::MouseMoved) {
      mousePos_m = {float(event_m.mouseMove.x), float(event_m.mouseMove.y)};
      mousePos_m =
          window.mapPixelToCoords({int(mousePos_m.x), int(mousePos_m.y)});
    }
    startGame_m.update(event_m, mousePos_m);
    chooseMode_m.update(event_m, mousePos_m);
    profiles_m.update(event_m, mousePos_m);
    settings_m.update(event_m, mousePos_m);
    exit_m.update(event_m, mousePos_m);
    if (event_m.type == sf::Event::Closed) {
      exit_m.reload();
      event_Close = true;
      event_MainMenu = false;
      prevScene.push(event_MainMenu);
    } else if (exit_m.wasActivated()) {
      exit_m.reload();
      event_Close = true;
      event_MainMenu = false;
      prevScene.push(event_MainMenu);
      calledByExitButton = true;
    } else if (startGame_m.wasActivated()) {
      startGame_m.reload();
      MMSoundtrackRef_m.stop();
      event_MainMenu = false;
      setBoolMode();
    } else if (chooseMode_m.wasActivated()) {
      chooseMode_m.reload();
      event_MainMenu = false;
      event_ChooseMode = true;
    } else if (profiles_m.wasActivated()) {
      profiles_m.reload();
      //            event_MainMenu = false;
      //            event_Profiles = true;
      comingSoon_m.show(true);
    } else if (settings_m.wasActivated()) {
      settings_m.reload();
      event_MainMenuSettings = true;
      prevScene.push(event_MainMenu);
      event_MainMenu = false;
    }
  }
}

template <class T>
void MainMenu::displayObjects(T& window, TechInfo& techInfo,
                              bool makeScreenshot) {
  window.clear();

  window.draw(pacmanTitle_m);
  window.draw(mainMenu_m);
  window.draw(startGame_m);
  window.draw(chooseMode_m);
  window.draw(profiles_m);
  window.draw(settings_m);
  window.draw(exit_m);
  window.draw(comingSoon_m);
  if (!makeScreenshot) {
    window.draw(techInfo);
  }

  window.display();
}
template void MainMenu::displayObjects<MyWindow>(MyWindow& window,
                                                 TechInfo& techInfo,
                                                 bool makeScreenshot);
template void MainMenu::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void MainMenu::makeScreenshot(MyWindow& window, TechInfo& techInfo) {
  window.rendTex.create(viewWidthPix_i, viewHeightPix_i);
  displayObjects(window.rendTex, techInfo, true);
  window.makeScreenshot();
}

void MainMenu::setBoolMode() {
  if (currMode.mode == CurrMode::classicGame && currMode.up == CurrMode::one)
    event_ClassicGame = true;
  else {
    std::cerr << "incorrect mode\n";
    exit(110);
  }
}