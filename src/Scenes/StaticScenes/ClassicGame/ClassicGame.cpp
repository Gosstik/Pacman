#include "ClassicGame.h"

ClassicGame::ClassicGame(Music& music)
    : CGMusic_m(music, level_m),
      pacman_m(level_m),
      ghosts_m(map_m, level_m),
      bonuses_m(map_m, pacman_m, level_m),
      CGLabels_m(pacman_m, score_m, bonuses_m, level_m),
      intermissions_m(CGMusic_m),
      preview_m(pacman_m, ghosts_m, bonuses_m, music) {}

void ClassicGame::open(MyWindow& window,
                       std::stack<std::reference_wrapper<bool>>& prevScene,
                       TechInfo& techInfo) {
  reloadAfterOpen(window, techInfo);
  while (event_ClassicGame) {
    techInfo.handleTime();
    while (techInfo.runCycle()) {
      techInfo.reloadRunCycle();
      handleEvents(window, prevScene);
      if (!event_ClassicGame) break;

      /// updating and drawing
      if (preview_m.isPlaying()) {
        preview_m.update(dTime_m);
        if (!preview_m.isPlaying()) {
          CGLabels_m.turnOffPreview();
          CGMusic_m.startGameSound.play();
        }
      } else if (CGLabels_m.startLevelAnimation() ||
                 CGLabels_m.continueLevelAnimation()) {
        CGLabels_m.update(map_m, score_m, CGMusic_m, dTime_m);
      } else if (CGLabels_m.gameOverAnimation()) {
        CGLabels_m.update(map_m, score_m, CGMusic_m, dTime_m);
        if (!CGLabels_m.gameOverAnimation()) {
          event_ClassicGame = false;
          prevScene.push(event_ClassicGame);
          event_NewGame = true;
          event_GameOver = true;
          break;
        }
      } else if (endLevel_m) {
        if (pauseBeforeEndLevelAnimation) {
          dTimeAccum_m += dTime_m;
          if (dTimeAccum_m >= timePauseBeforeEndLevelAnimation) {
            pauseBeforeEndLevelAnimation = false;
            CGLabels_m.setEndLevelAnimation(map_m);
            dTimeAccum_m = 0;
          }
        } else if (CGLabels_m.endLevelAnimation()) {
          CGLabels_m.update(map_m, score_m, CGMusic_m, dTime_m);
          if (!CGLabels_m.endLevelAnimation()) {
            endLevel_m = false;
            dTimeAccum_m = 0;
            if (level_m % intermissionFrequency_m == 0) {
              CGLabels_m.intermissionIsPlaying_m = true;
              intermissions_m.reloadCurrent(CGMusic_m);
            } else
              reloadNextLevel();
            continue;
          }
        }
      } else if (pacman_m.isKilled()) {
        CGLabels_m.updateUpLabel(dTime_m);
        if (pauseBeforeKillAnimation_m) {
          dTimeAccum_m += dTime_m;
          if (dTimeAccum_m >= timePauseBeforeKillAnimation) {
            pauseBeforeKillAnimation_m = false;
            executePacmanKillAnimation_m = true;  // for animation
            dTimeAccum_m = 0;
            CGMusic_m.deathStartSound.play();
          }
        } else if (!pacman_m.killAnimationEnded()) {
          pacman_m.update(window, map_m, CGMusic_m, dTime_m);
          if (pacman_m.killAnimationEnded()) {
            executePacmanKillAnimation_m = false;  // for animation
            pauseAfterKillAnimation_m = true;
          }
        } else if (pauseAfterKillAnimation_m) {
          dTimeAccum_m += dTime_m;
          if (dTimeAccum_m >= timePauseAfterKillAnimation) {
            pauseAfterKillAnimation_m = false;
            dTimeAccum_m = 0;
            reloadAfterPacmanDeath();
            continue;
          }
        }
      } else if (someGhostWasKilled_m) {
        if (CGMusic_m.eatGhostSound.getStatus() == sf::SoundSource::Stopped) {
          someGhostWasKilled_m = false;
          ghosts_m.removeJustBeenKilled();
          if (ghosts_m.smbIsMovingToCage()) {
            CGMusic_m.bigGumEffectSound.pause();
            CGMusic_m.moveToCageSound.setLoop(true);
            CGMusic_m.moveToCageSound.play();
          }
          // in "else" condition ghosts still under big gum effect because of
          // pause while killing ghost
          else
            CGMusic_m.bigGumEffectSound.play();
        }
      } else if (CGLabels_m.intermissionIsPlaying_m) {
        intermissions_m.updateCurrent(CGMusic_m, dTime_m);
        if (intermissions_m.currentEnded()) {
          CGLabels_m.intermissionIsPlaying_m = false;
          reloadNextLevel();
        }
      } else {
        map_m.update(window);
        pacman_m.update(window, map_m, CGMusic_m, dTime_m);
        ghosts_m.update(window, map_m, pacman_m, CGMusic_m,
                        bonuses_m.allGumsLeft(), dTime_m);
        bonuses_m.update(map_m, pacman_m, ghosts_m, score_m, CGMusic_m,
                         ghosts_m.bigGumEffect(), dTime_m);
        handleExtraLife();
        CGLabels_m.update(map_m, score_m, CGMusic_m, dTime_m);
        handleGumsCount();
        ghosts_m.handlePacmanCollision(map_m, pacman_m, score_m, CGMusic_m,
                                       someGhostWasKilled_m,
                                       pauseBeforeKillAnimation_m);
        if (pauseBeforeKillAnimation_m) bonuses_m.showBigGums();
      }
    }

    if (techInfo.runDisplay()) {
      techInfo.reloadRunDisplay();
      displayObjects(window, techInfo, false);
    }
  }

  if (!event_GameOver) {
    if (CGMusic_m.startGameSound.getStatus() == sf::SoundSource::Playing)
      CGMusic_m.startGameSound.pause();
    else if (!CGLabels_m.startLevelAnimation() &&
             !CGLabels_m.continueLevelAnimation())
      CGMusic_m.pauseAllMusic();
  }

  makeScreenshot(window, techInfo);
}

void ClassicGame::reloadAfterOpen(MyWindow& window, TechInfo& techInfo) {
  if (event_NewGame) {
    event_NewGame = false;
    reloadNewGame();
  } else if (CGLabels_m.startLevelAnimation())
    CGLabels_m.continueStartLevelAnimation(CGMusic_m);
  else
    CGLabels_m.setContinueLevelAnimation(CGMusic_m);

  dTime_m = techInfo.getCycleDuration();
}

void ClassicGame::handleEvents(
    MyWindow& window, std::stack<std::reference_wrapper<bool>>& prevScene) {
  while (window.pollEvent(event_m)) {
    if (event_m.type == sf::Event::Closed) {
      event_ClassicGame = false;
      prevScene.push(event_ClassicGame);
      event_Close = true;
    } else if (event_m.type == sf::Event::KeyPressed) {
      if (event_m.key.code == sf::Keyboard::Space && preview_m.isPlaying()) {
        preview_m.turnOff();
      } else if (event_m.key.code == sf::Keyboard::Escape) {
        event_ClassicGame = false;
        prevScene.push(event_ClassicGame);
        event_Pause = true;
      } else if (!preview_m.isPlaying())
        pacman_m.updateDirection(event_m);
    }
  }
}

template <class T>
void ClassicGame::displayObjects(T& window, TechInfo& techInfo,
                                 bool makeScreenshot) {
  window.clear();

  if (preview_m.isPlaying()) {
    window.draw(preview_m);
    window.draw(CGLabels_m);
  } else {
    if (CGLabels_m.intermissionIsPlaying_m) {
      window.draw(intermissions_m);
      window.draw(CGLabels_m);
    } else {
      window.draw(map_m);
      window.draw(CGLabels_m);
      window.draw(bonuses_m);
      ghosts_m.drawKillScore(window);
      bool drawGhosts = !CGLabels_m.gameOverAnimation() &&
                        !executePacmanKillAnimation_m &&
                        !pauseAfterKillAnimation_m &&
                        !CGLabels_m.endLevelAnimation() && !event_GameOver;
      if (drawGhosts) ghosts_m.drawWithBigGumEffect(window);
      if (!CGLabels_m.gameOverAnimation() && !pacman_m.killAnimationEnded() &&
          !someGhostWasKilled_m)
        window.draw(pacman_m);
      if (drawGhosts) ghosts_m.drawWithoutBigGumEffect(window);
      window.draw(portals_m);
    }
  }
  if (!makeScreenshot) window.draw(techInfo);

  window.display();
}
template void ClassicGame::displayObjects<MyWindow>(MyWindow& window,
                                                    TechInfo& techInfo,
                                                    bool makeScreenshot);
template void ClassicGame::displayObjects<sf::RenderTexture>(
    sf::RenderTexture& window, TechInfo& techInfo, bool makeScreenshot);

void ClassicGame::makeScreenshot(MyWindow& window, TechInfo& techInfo) {
  window.rendTex.create(viewWidthPix_i, viewHeightPix_i);
  displayObjects(window.rendTex, techInfo, true);

  if (event_Pause || event_GameOver) calledFromGameMode = true;
  window.makeScreenshot();
}

void ClassicGame::handleExtraLife() {
  if (score_m.getExtraLifeScore() >= scoreForExtraLife) {
    score_m.takeRemOfExtraLifeScore();
    if (pacman_m.getExtraLives() < pacmanMaxNumOfExtraLives) {
      CGMusic_m.extraLifeSound.play();
      pacman_m.addExtraLife();
      CGLabels_m.addLife();
    }
  }
}

void ClassicGame::handleGumsCount() {
  int currSiren = CGMusic_m.currSiren;
  if (CGMusic_m.bigGumEffectSound.getStatus() != sf::SoundSource::Playing &&
      CGMusic_m.moveToCageSound.getStatus() != sf::SoundSource::Playing) {
    if ((currSiren == 0 &&
         bonuses_m.gumsLeft() + bonuses_m.bigGumsLeft() <= 120) ||  // 120
        (currSiren == 1 &&
         bonuses_m.gumsLeft() + bonuses_m.bigGumsLeft() <= 60) ||  // 60
        (currSiren == 2 &&
         bonuses_m.gumsLeft() + bonuses_m.bigGumsLeft() <= 35) ||  // 35
        (currSiren == 3 &&
         bonuses_m.gumsLeft() + bonuses_m.bigGumsLeft() <= 15)) {  // 15
      CGMusic_m.sirens_m[currSiren].stop();
      CGMusic_m.sirens_m[++CGMusic_m.currSiren].play();
    }
  }

  if (bonuses_m.allGumsLeft() == 0) setEndLevel();
}

void ClassicGame::updateIntermissionFrequency() {
  if ((level_m - 1) % 3 == 0) intermissionFrequency_m += 1;
}

void ClassicGame::setGameOver() { CGLabels_m.setGameOverAnimation(); }

void ClassicGame::setEndLevel() {
  CGMusic_m.stopAllMusic();
  pacman_m.setEndLevelAnimation();
  endLevel_m = true;
  pauseBeforeEndLevelAnimation = true;
}

void ClassicGame::setDefaultVariables() {
  pauseBeforeKillAnimation_m = false;
  pauseAfterKillAnimation_m = false;
  pauseBeforeEndLevelAnimation = false;
  executePacmanKillAnimation_m = false;
  endLevel_m = false;
  someGhostWasKilled_m = false;

  dTimeAccum_m = 0;
}

void ClassicGame::reloadAfterPacmanDeath() {
  if (pacman_m.getExtraLives() == 0) {
    setGameOver();
    return;
  }
  pacman_m.reloadAfterDeath();
  ghosts_m.reloadAfterPacmanDeath();
  bonuses_m.reloadAfterPacmanDeath(map_m);
  CGLabels_m.reloadAfterPacmanDeath(CGMusic_m);

  setDefaultVariables();
}

void ClassicGame::reloadNextLevel() {
  ++level_m;
  map_m.reloadNextLevel();
  pacman_m.reloadNextLevel(level_m);
  ghosts_m.reloadNextLevel(level_m);
  bonuses_m.reloadNextLevel(map_m, level_m);
  CGLabels_m.reloadNextLevel(bonuses_m, CGMusic_m, level_m);
  CGMusic_m.reloadNextLevel(level_m);

  updateIntermissionFrequency();

  setDefaultVariables();
}

void ClassicGame::reloadNewGame() {
  level_m = 1;
  map_m.reloadNewGame();
  pacman_m.reloadNewGame();
  ghosts_m.reloadNewGame();
  bonuses_m.reloadNewGame(map_m);
  score_m.reloadNewGame();
  CGLabels_m.reloadNewGame(pacman_m, bonuses_m, score_m);
  CGMusic_m.reloadNewGame();
  preview_m.reloadNewGame();

  intermissionFrequency_m = 1;

  setDefaultVariables();
}

void ClassicGame::saveData() { score_m.saveData(); }