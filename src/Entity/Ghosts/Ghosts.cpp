#include "Ghosts.h"

void Ghosts::drawKillScore(sf::RenderTarget& target) const {
  if (blinky_m.justBeenKilled_m || pinky_m.justBeenKilled_m ||
      inky_m.justBeenKilled_m || clyde_m.justBeenKilled_m)
    target.draw(killScoreSpr_m);
}

void Ghosts::drawWithoutBigGumEffect(sf::RenderTarget& target) const {
  if (!blinky_m.underBigGumEffect() && !blinky_m.justBeenKilled_m)
    target.draw(blinky_m);
  if (!pinky_m.underBigGumEffect() && !pinky_m.justBeenKilled_m)
    target.draw(pinky_m);
  if (!inky_m.underBigGumEffect() && !inky_m.justBeenKilled_m)
    target.draw(inky_m);
  if (!clyde_m.underBigGumEffect() && !clyde_m.justBeenKilled_m)
    target.draw(clyde_m);
}

void Ghosts::drawWithBigGumEffect(sf::RenderTarget& target) const {
  if (blinky_m.underBigGumEffect() && !blinky_m.justBeenKilled_m)
    target.draw(blinky_m);
  if (pinky_m.underBigGumEffect() && !pinky_m.justBeenKilled_m)
    target.draw(pinky_m);
  if (inky_m.underBigGumEffect() && !inky_m.justBeenKilled_m)
    target.draw(inky_m);
  if (clyde_m.underBigGumEffect() && !clyde_m.justBeenKilled_m)
    target.draw(clyde_m);
}

Ghosts::Ghosts(Map& map, int level)
    : blinky_m(ghostsTex_m, level),
      pinky_m(ghostsTex_m, level),
      inky_m(ghostsTex_m, level),
      clyde_m(ghostsTex_m, level) {
  ghostsImg_m.loadFromFile(imageGhostsFilename_m);
  ghostsImg_m.createMaskFromColor(sf::Color::Black);
  ghostsTex_m.loadFromImage(ghostsImg_m);

  killScoreImg_m.loadFromFile(imageKillScoreFilename_m);
  killScoreImg_m.createMaskFromColor(sf::Color::Black);
  killScoreTex_m.loadFromImage(killScoreImg_m);
  killScoreSpr_m.setTexture(killScoreTex_m);
  killScoreSpr_m.setTextureRect({0, 0, entitySize_i, entitySize_i});
  killScoreSpr_m.setRotation(90.0f);
  killScoreSpr_m.setOrigin(killScoreSpr_m.getLocalBounds().width / 2.0f,
                           killScoreSpr_m.getLocalBounds().height / 2.0f);

  cagePopulation_m.resize(3);
  setStartCagePopulation();
  updateCageLimits(level);

  tm_m.updateTimeConsts(level);

  // speed
  updateSpeed(level);
  blinky_m.updateSpeed(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  pinky_m.updateSpeed(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  inky_m.updateSpeed(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  clyde_m.updateSpeed(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
}

void Ghosts::updateCageLimits(int level) {
  centralCageLimitDots_m = 0;
  if (level == 1) {
    leftCageCellLimitDots_m = leftCageCellLimitDotsVec_m[0];
    rightCageCellLimitDots_m = rightCageCellLimitDotsVec_m[0];
  } else if (level == 2) {
    leftCageCellLimitDots_m = leftCageCellLimitDotsVec_m[1];
    rightCageCellLimitDots_m = rightCageCellLimitDotsVec_m[1];
  } else
    leftCageCellLimitDots_m = rightCageCellLimitDots_m = 0;
}

void Ghosts::setScatterMode(Map& map, Pacman& pacman) {
  currMode_m = scatter;
  blinky_m.setScatterMode(map, pacman);
  pinky_m.setScatterMode(map, pacman);
  inky_m.setScatterMode(map, pacman);
  clyde_m.setScatterMode(map, pacman);
}

void Ghosts::setChaseMode(Map& map, Pacman& pacman) {
  currMode_m = chase;
  blinky_m.setChaseMode(map, pacman, blinky_m);
  pinky_m.setChaseMode(map, pacman, blinky_m);
  inky_m.setChaseMode(map, pacman, blinky_m);
  clyde_m.setChaseMode(map, pacman, blinky_m);
}

void Ghosts::setStartCagePopulation() {
  cagePopulation_m[0] = &pinky_m;
  cagePopulation_m[1] = &inky_m;
  cagePopulation_m[2] = &clyde_m;
}

// Ghosts::Ghosts(const Ghosts& other) {
//
// }
//
// Ghosts& Ghosts::operator=(const Ghosts& other) {
//
// }
//
// Ghosts::Ghosts(Ghosts&& other) noexcept {
//
// }
//
// Ghosts& Ghosts::operator=(Ghosts&& other) noexcept {
//
// }

void Ghosts::update(MyWindow& window, Map& map, Pacman& pacman,
                    ClassicGameMusic& CGMusic, int allGumsLeft, float dTime) {
  handleCage(dTime);
  handleBigGumEffect(CGMusic, dTime);
  handleMode(map, pacman, dTime);
  blinky_m.handleElroyState(allGumsLeft);
  blinky_m.update(window, map, pacman, blinky_m, st_m, tm_m, CGMusic,
                  currMode_m, cagePopulation_m, releasingGhost_m, dTime);
  pinky_m.update(window, map, pacman, blinky_m, st_m, tm_m, CGMusic, currMode_m,
                 cagePopulation_m, releasingGhost_m, dTime);
  inky_m.update(window, map, pacman, blinky_m, st_m, tm_m, CGMusic, currMode_m,
                cagePopulation_m, releasingGhost_m, dTime);
  clyde_m.update(window, map, pacman, blinky_m, st_m, tm_m, CGMusic, currMode_m,
                 cagePopulation_m, releasingGhost_m, dTime);

  handleMoveToCageMusic(CGMusic);
}

void Ghosts::resetCageAccumTime() { tm_m.accumTimeNextRelease = 0; }

void Ghosts::pacmanAteBonus() {
  ++dotsCounter_m;
  tm_m.accumTimeNextRelease = 0;
}

void Ghosts::handleCage(float dTime) {
  if (releasingGhost_m != nullptr)
    if (releasingGhost_m->isAlive()) releasingGhost_m = nullptr;

  if (releasingGhost_m == nullptr) {
    if (dotsCounter_m < rightCageCellLimitDots_m) {
      tm_m.accumTimeNextRelease += dTime;
      if (cagePopulation_m[0] != nullptr) {
        if (cagePopulation_m[0]->isInPlace()) {
          if (dotsCounter_m >= centralCageLimitDots_m ||
              tm_m.accumTimeNextRelease >= tm_m.constTimeNextRelease) {
            tm_m.accumTimeNextRelease = 0;
            cagePopulation_m[0]->releaseFromCage();
            releasingGhost_m = cagePopulation_m[0];
            cagePopulation_m[0] = nullptr;
          }
        }
        return;  // return automatically accomplished
      } else if (cagePopulation_m[1] != nullptr &&
                 cagePopulation_m[1]->isInPlace()) {
        if (dotsCounter_m >= leftCageCellLimitDots_m ||
            tm_m.accumTimeNextRelease >= tm_m.constTimeNextRelease) {
          tm_m.accumTimeNextRelease = 0;
          cagePopulation_m[1]->releaseFromCage();
          releasingGhost_m = cagePopulation_m[1];
          cagePopulation_m[1] = nullptr;
        }
      } else if (cagePopulation_m[2] != nullptr &&
                 cagePopulation_m[2]->isInPlace()) {
        if (tm_m.accumTimeNextRelease >= tm_m.constTimeNextRelease) {
          blinky_m.clydeLeftCage_m = true;
          tm_m.accumTimeNextRelease = 0;
          cagePopulation_m[2]->releaseFromCage();
          releasingGhost_m = cagePopulation_m[2];
          cagePopulation_m[2] = nullptr;
        }
      }
    }
    // else needs only for optimisation (we don't need to track dotCounter and
    // timeNextRelease)
    else {
      if (cagePopulation_m[0] != nullptr) {
        if (cagePopulation_m[0]->isInPlace()) {
          cagePopulation_m[0]->releaseFromCage();
          releasingGhost_m = cagePopulation_m[0];
          cagePopulation_m[0] = nullptr;
        }
        return;
      } else {
        for (size_t i = 1; i < cagePopulation_m.size(); ++i) {
          if (cagePopulation_m[i] != nullptr &&
              cagePopulation_m[i]->isInPlace()) {
            if (i == 2) blinky_m.clydeLeftCage_m = true;
            cagePopulation_m[i]->releaseFromCage();
            releasingGhost_m = cagePopulation_m[i];
            cagePopulation_m[i] = nullptr;
            return;
          }
        }
      }
    }
  }
}

void Ghosts::handleMode(Map& map, Pacman& pacman, float dTime) {
  if (currMode_m != frightened && tm_m.currWave != 4) {
    tm_m.accumTimeMode += dTime;
    if (currMode_m == scatter) {
      if (tm_m.accumTimeMode >=
          tm_m.constScatterTimes[tm_m.lvlModeTime][tm_m.currWave]) {
        ++tm_m.currWave;
        tm_m.accumTimeMode = 0;
        setChaseMode(map, pacman);
      }
    } else if (tm_m.accumTimeMode >=
               tm_m.constChaseTimes[tm_m.lvlModeTime][tm_m.currWave]) {
      tm_m.accumTimeMode = 0;
      setScatterMode(map, pacman);
    }
  }
}

void Ghosts::handleMoveToCageMusic(ClassicGameMusic& CGMusic) const {
  if (CGMusic.moveToCageSound.getLoop() && !smbIsMovingToCage())
    CGMusic.moveToCageSound.setLoop(false);
}

void Ghosts::handleBigGumEffect(ClassicGameMusic& CGMusic, float dTime) {
  bool someIsMovingToCage = smbIsMovingToCage();

  if (st_m.bigGumEffect) {
    tm_m.accumTimeBigGumEffect += dTime;
    if (tm_m.accumTimeBigGumEffect >= tm_m.frightTime) {
      CGMusic.bigGumEffectSound.stop();
      if (CGMusic.moveToCageSound.getStatus() != sf::SoundSource::Playing)
        CGMusic.sirens_m[CGMusic.currSiren].play();
      currMode_m = pausedMode_m;
      st_m.bigGumEffect = false;
      tm_m.accumTimeBigGumEffect = 0;
      killScoreIndex_m = 0;
      return;
    }
    if (CGMusic.moveToCageSound.getStatus() == sf::SoundSource::Playing) {
      if (!someIsMovingToCage && CGMusic.moveToCageSound.getLoop())
        CGMusic.moveToCageSound.setLoop(false);
    }
    // it means that now bigGumEffectSound is playing
    else if (CGMusic.bigGumEffectSound.getStatus() !=
             sf::SoundSource::Playing) {
      CGMusic.bigGumEffectSound.play();
    }
  } else if (CGMusic.moveToCageSound.getStatus() == sf::SoundSource::Playing) {
    if (!someIsMovingToCage) {
      CGMusic.moveToCageSound.stop();
      CGMusic.sirens_m[CGMusic.currSiren].play();
    }
  }
}

void Ghosts::updateSpeed(int level) {
  size_t size = lvlChangeSpeedRatio.size();
  for (size_t i = 0; i < size; ++i) {
    if ((i == size - 1) || level < lvlChangeSpeedRatio[i + 1]) {
      normalSpeed_m = maxSpeed * normalSpeedRatio_m[i];
      tunnelSpeed_m = maxSpeed * tunnelSpeedRatio_m[i];
      frightSpeed_m = maxSpeed * frightSpeedRatio_m[i];
      break;
    }
  }
}

void Ghosts::setBigGumEffect(Map& map, ClassicGameMusic& CGMusic) {
  if (tm_m.frightTime != 0) {
    CGMusic.sirens_m[CGMusic.currSiren].stop();
    if (CGMusic.moveToCageSound.getStatus() != sf::SoundSource::Playing &&
        CGMusic.bigGumEffectSound.getStatus() != sf::SoundSource::Playing)
      CGMusic.bigGumEffectSound.play();
    if (currMode_m != GhostsMode::frightened) {
      pausedMode_m = currMode_m;
      currMode_m = frightened;
      st_m.bigGumEffect = true;
    }
    tm_m.accumTimeBigGumEffect = 0;

    blinky_m.setBigGumEffect(map);
    pinky_m.setBigGumEffect(map);
    inky_m.setBigGumEffect(map);
    clyde_m.setBigGumEffect(map);
  } else {
    blinky_m.simpleReverseDir();
    pinky_m.simpleReverseDir();
    inky_m.simpleReverseDir();
    clyde_m.simpleReverseDir();
  }
}

bool Ghosts::smbIsMovingToCage() const {
  if (blinky_m.isMovingToCage() || pinky_m.isMovingToCage() ||
      inky_m.isMovingToCage() || clyde_m.isMovingToCage())
    return true;
  return false;
}

bool Ghosts::bigGumEffect() const { return st_m.bigGumEffect; }

void Ghosts::handlePacmanCollision(Map& map, Pacman& pacman, Score& score,
                                   ClassicGameMusic& CGMusic,
                                   bool& someGhostWasKilled_m,
                                   bool& pauseBeforeKillAnimation) {
  handlePacmanCollisionG(blinky_m, map, pacman, score, CGMusic,
                         someGhostWasKilled_m, pauseBeforeKillAnimation);
  if (!someGhostWasKilled_m && !pauseBeforeKillAnimation)
    handlePacmanCollisionG(pinky_m, map, pacman, score, CGMusic,
                           someGhostWasKilled_m, pauseBeforeKillAnimation);
  if (!someGhostWasKilled_m && !pauseBeforeKillAnimation)
    handlePacmanCollisionG(inky_m, map, pacman, score, CGMusic,
                           someGhostWasKilled_m, pauseBeforeKillAnimation);
  if (!someGhostWasKilled_m && !pauseBeforeKillAnimation)
    handlePacmanCollisionG(clyde_m, map, pacman, score, CGMusic,
                           someGhostWasKilled_m, pauseBeforeKillAnimation);
}

void Ghosts::removeJustBeenKilled() {
  if (blinky_m.justBeenKilled_m)
    blinky_m.justBeenKilled_m = false;
  else if (pinky_m.justBeenKilled_m)
    pinky_m.justBeenKilled_m = false;
  else if (inky_m.justBeenKilled_m)
    inky_m.justBeenKilled_m = false;
  else if (clyde_m.justBeenKilled_m)
    clyde_m.justBeenKilled_m = false;
}

const sf::Texture* Ghosts::getGhostsTexture() const { return &ghostsTex_m; }

const sf::Texture* Ghosts::getKillScoreTexture() const {
  return &killScoreTex_m;
}

void Ghosts::reloadAfterPacmanDeath() {
  blinky_m.reloadAfterPacmanDeath();
  pinky_m.reloadAfterPacmanDeath();
  inky_m.reloadAfterPacmanDeath();
  clyde_m.reloadAfterPacmanDeath();

  // cage
  dotsCounter_m = 0;
  setStartCagePopulation();
  releasingGhost_m = nullptr;
  centralCageLimitDots_m = 7;
  leftCageCellLimitDots_m = 17;
  rightCageCellLimitDots_m = 32;

  // other
  tm_m.setDefault();
  st_m.setDefault();
  killScoreIndex_m = 0;
  currMode_m = pausedMode_m = scatter;
}

void Ghosts::reloadNextLevel(
    int level) {  // todo: reloadNextLevel == reloadNewGame
  updateCageLimits(level);
  tm_m.updateTimeConsts(level);
  updateSpeed(level);

  // depend on speed
  blinky_m.reloadNextLevel(level, normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  pinky_m.reloadNextLevel(level, normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  inky_m.reloadNextLevel(level, normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  clyde_m.reloadNextLevel(level, normalSpeed_m, tunnelSpeed_m, frightSpeed_m);

  dotsCounter_m = 0;
  setStartCagePopulation();
  releasingGhost_m = nullptr;

  tm_m.setDefault();
  st_m.setDefault();
  killScoreIndex_m = 0;
  currMode_m = pausedMode_m = scatter;
}

void Ghosts::reloadNewGame() {
  updateCageLimits(1);
  tm_m.updateTimeConsts(1);
  updateSpeed(1);

  // depend on speed
  blinky_m.reloadNewGame(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  pinky_m.reloadNewGame(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  inky_m.reloadNewGame(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);
  clyde_m.reloadNewGame(normalSpeed_m, tunnelSpeed_m, frightSpeed_m);

  dotsCounter_m = 0;
  setStartCagePopulation();
  releasingGhost_m = nullptr;

  tm_m.setDefault();
  st_m.setDefault();
  killScoreIndex_m = 0;
  currMode_m = pausedMode_m = scatter;
}