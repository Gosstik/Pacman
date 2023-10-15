#include "ClassicGameLabels.h"

void ClassicGameLabels::draw(sf::RenderTarget& target,
                             sf::RenderStates states) const {
  if (!intermissionIsPlaying_m) {
    if (drawOneUp_m) target.draw(oneUp_m);
    target.draw(scoreNum_m);
    target.draw(highScore_m);
    target.draw(highScoreNum_m);
    if (showLevel_b) {
      target.draw(level_m);
      target.draw(levelNum_m);
    }
    if (previewIsPlaying_m) return;
  }
  for (auto& elem : livesLst_m) target.draw(elem);
  for (auto& elem : fruitsLst_m) target.draw(elem);
  if (startLevelAnimation_m || continueLevelAnimation_m) {
    target.draw(ready_m);
    if (CDNum_m <= pauseBeforeContinueLevel && showCountdown_b)
      target.draw(countdownNum_m);
  } else if (gameOverAnimation_m)
    target.draw(gameOver_m);
}

ClassicGameLabels::ClassicGameLabels(Pacman& pacman, Score& score,
                                     Bonuses& bonuses, int level)
    : oneUp_m("1up", textScale_m, textScale_m, oneUpPosX_m, oneUpPosY_m, true),
      scoreNum_m(score.getScoreNum(), textScale_m, textScale_m, scoreNumPosX_m,
                 scoreNumPosY_m, true),
      highScore_m("high score", textScale_m, textScale_m, highScorePosX_m,
                  highScorePosY_m, true),
      highScoreNum_m(score.getHighScoreNum(), textScale_m, textScale_m,
                     highScoreNumPosX_m, highScoreNumPosY_m, true),
      level_m("level", textScale_m, textScale_m, levelPosX_m, levelPosY_m,
              true),
      levelNum_m(level, textScale_m, textScale_m, levelNumPosX_m,
                 levelNumPosY_m, true),
      ready_m("ready!", textScale_m, textScale_m, readyPosX_m, readyPosY_m,
              true, readyColor_m),
      countdownNum_m(CDNum_m, countdownNumScale_m, countdownNumScale_m,
                     countdownNumPosX_m, countdownNumPosY_m, true,
                     countdownNumColor_m),
      gameOver_m("game over", textScale_m, textScale_m, gameOverPosX_m,
                 gameOverPosY_m, true, gameOverColor_m) {
  // creating images of getExtraLives
  livesSpr_m.setTexture(*pacman.getTexture());
  livesSpr_m.setTextureRect(
      {livesRectLeft_m, livesRectTop_m, entitySize_i, entitySize_i});
  livesSpr_m.setOrigin(livesSpr_m.getLocalBounds().width / 2.0f,
                       livesSpr_m.getLocalBounds().height / 2.0f);

  livesLst_m.resize(pacman.getExtraLives());
  for (int i = 0; i < pacman.getExtraLives(); ++i) {
    livesSpr_m.setPosition(livesStartPosX_m + float(i) * livesOffsetX,
                           livesStartPosY_m);
    livesLst_m.emplace_back(livesSpr_m);
  }

  // creating images of fruits
  fruitSpr_m.setTexture(
      *bonuses.getFruitsTexture());  // todo: there can be cause of errors
                                     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  fruitSpr_m.setTextureRect(bonuses.getFruitTextureRect());
  fruitSpr_m.setOrigin(fruitSpr_m.getLocalBounds().width / 2.0f,
                       fruitSpr_m.getLocalBounds().height / 2.0f);
  for (int currLvl = 1; currLvl <= level; ++currLvl)
    reloadFruitsLabelNextLevel(bonuses);
}

void ClassicGameLabels::update(Map& map, Score& score,
                               ClassicGameMusic& CGMusic, float dTime) {
  updateUpLabel(dTime);
  if (std::to_string(score.getScoreNum()) != scoreNum_m.getString())
    scoreNum_m.setString(score.getScoreNum());
  if (std::to_string(score.getHighScoreNum()) != highScoreNum_m.getString())
    highScoreNum_m.setString(score.getScoreNum());

  if (startLevelAnimation_m) {
    handleCountdown(startLevelAnimation_m, dTime);
    if (!startLevelAnimation_m) CGMusic.sirens_m[CGMusic.currSiren].play();
  } else if (continueLevelAnimation_m) {
    handleCountdown(continueLevelAnimation_m, dTime);
    if (!continueLevelAnimation_m) CGMusic.resumeAllMusic();
  } else if (gameOverAnimation_m) {
    accumTime_m += dTime;
    if (accumTime_m >= timeGameOverAnimation) {
      gameOverAnimation_m = false;
      accumTime_m = 0;
    }
  } else if (endLevelAnimation_m) {
    accumTime_m += dTime;
    if (accumTime_m >= timeEndLevelAnimation) {
      endLevelAnimation_m = false;
      accumTimeChangeMapColor_m = accumTime_m = 0;
      map.setDefault();
    }
    accumTimeChangeMapColor_m += dTime;
    if (accumTimeChangeMapColor_m >= timeChangeMapColor) {
      while (accumTimeChangeMapColor_m >= timeChangeMapColor) {
        accumTimeChangeMapColor_m -= timeChangeMapColor;
        map.changeColor();
      }
    }
  }
}

void ClassicGameLabels::handleCountdown(bool& state, float dTime) {
  accumTime_m += dTime;
  if (accumTime_m >= mcsInSec) {
    while (accumTime_m >= mcsInSec) {
      accumTime_m -= mcsInSec;
      --CDNum_m;
    }
    if (CDNum_m < 1) {
      state = false;
      accumTime_m = 0;
    } else
      countdownNum_m.setString(CDNum_m);
  }
}

void ClassicGameLabels::reloadFruitsLabelNextLevel(Bonuses& bonuses) {
  if (fruitsLst_m.size() == maxFruitLabels_m) {
    fruitsLst_m.erase(fruitsLst_m.begin());
    fruitSpr_m.setTextureRect(bonuses.getFruitTextureRect());
    fruitsLst_m.emplace_back(fruitSpr_m);
    float i = 0;
    for (auto& spr : fruitsLst_m) {
      spr.setPosition(firstFruitPosX - i * entitySize_f, firstFruitPosY);
      i += 1.0f;
    }
  } else {
    if (fruitsLst_m.size() > maxFruitLabels_m) {
      std::cerr << "ClassicGameLabels::reloadNextLevel : fruitsLst_m.size() > "
                   "maxFruitLabels_m\n";
      exit(11);
    }
    fruitSpr_m.setTextureRect(bonuses.getFruitTextureRect());
    fruitsLst_m.emplace_back(fruitSpr_m);
    auto i = float(fruitsLst_m.size() - 1);
    fruitsLst_m.back().setPosition(firstFruitPosX - i * entitySize_f,
                                   firstFruitPosY);
  }
}

void ClassicGameLabels::setGameOverAnimation() {
  gameOverAnimation_m = true;
  accumTime_m = 0;
}

void ClassicGameLabels::setContinueLevelAnimation(ClassicGameMusic& CGMusic) {
  if (!intermissionIsPlaying_m && pauseBeforeContinueGame_b) {
    continueLevelAnimation_m = true;
    reloadCountdown(pauseBeforeContinueLevel);
  } else {
    CDNum_m = 0;
    accumTime_m = 0;
    continueLevelAnimation_m = false;
    CGMusic.resumeAllMusic();
  }
}

void ClassicGameLabels::setEndLevelAnimation(Map& map) {
  map.stopShowCageEntrance();
  map.changeColor();
  endLevelAnimation_m = true;
}

void ClassicGameLabels::continueStartLevelAnimation(ClassicGameMusic& CGMusic) {
  if (CGMusic.startGameSound.getStatus() == sf::SoundSource::Paused)
    CGMusic.startGameSound.play();
  if (pauseBeforeContinueGame_b) reloadCountdown(pauseBeforeContinueLevel);
}

void ClassicGameLabels::turnOffPreview() { previewIsPlaying_m = false; }

bool ClassicGameLabels::startLevelAnimation() const {
  return startLevelAnimation_m;
}

bool ClassicGameLabels::gameOverAnimation() const {
  return gameOverAnimation_m;
}

bool ClassicGameLabels::continueLevelAnimation() const {
  return continueLevelAnimation_m;
}

bool ClassicGameLabels::endLevelAnimation() const {
  return endLevelAnimation_m;
}

void ClassicGameLabels::reloadCountdown(int seconds) {
  if (seconds >= CDNum_m) {
    CDNum_m = seconds;
    if (CDNum_m <= pauseBeforeContinueLevel) countdownNum_m.setString(CDNum_m);
    accumTime_m = 0;
  }
}

void ClassicGameLabels::addLife() {
  livesSpr_m.setPosition(
      livesStartPosX_m + float(livesLst_m.size()) * livesOffsetX,
      livesStartPosY_m);
  livesLst_m.emplace_back(livesSpr_m);
}

void ClassicGameLabels::updateUpLabel(float dTime) {
  accumTimeDrawOneUp_m += dTime;
  while (accumTimeDrawOneUp_m >= constTimeDrawOneUp_m) {
    accumTimeDrawOneUp_m -= constTimeDrawOneUp_m;
    drawOneUp_m = !drawOneUp_m;
  }
}

void ClassicGameLabels::reloadAfterPacmanDeath(ClassicGameMusic& CGMusic) {
  if (livesLst_m.empty())
    std::cerr
        << "ClassicGameLabels::reloadAfterPacmanDeath : livesLst_m.empty()\n";
  else
    livesLst_m.erase(--livesLst_m.end());
  startLevelAnimation_m = true;
  accumTimeDrawOneUp_m = 0;
  drawOneUp_m = true;
  reloadCountdown(pauseBeforeContinueLevel);
}

/// BONUSES MUST BE UPDATED BEFORE THIS
void ClassicGameLabels::reloadNextLevel(Bonuses& bonuses,
                                        ClassicGameMusic& CGMusic, int level) {
  // updateDisplayValue level_m
  levelNum_m.setString(level);

  // updateDisplayValue fruits
  reloadFruitsLabelNextLevel(bonuses);

  startLevelAnimation_m = true;
  accumTimeDrawOneUp_m = 0;
  drawOneUp_m = true;
  reloadCountdown(pauseBeforeContinueLevel);
}

void ClassicGameLabels::reloadNewGame(Pacman& pacman, Bonuses& bonuses,
                                      Score& score) {
  scoreNum_m.setString(score.getScoreNum());
  highScoreNum_m.setString(score.getHighScoreNum());
  levelNum_m.setString(1);

  livesLst_m.clear();
  for (int i = 0; i < pacman.getExtraLives(); ++i) {
    livesSpr_m.setPosition(livesStartPosX_m + float(i) * livesOffsetX,
                           livesStartPosY_m);
    livesLst_m.emplace_back(livesSpr_m);
  }

  fruitsLst_m.clear();
  reloadFruitsLabelNextLevel(bonuses);

  if (pauseBeforeContinueGame_b)
    reloadCountdown(pauseBeforeNewGame);
  else
    CDNum_m = 5;

  accumTimeDrawOneUp_m = accumTimeChangeMapColor_m = accumTime_m = 0;

  startLevelAnimation_m = true;
  endLevelAnimation_m = false;
  gameOverAnimation_m = false;
  continueLevelAnimation_m = false;
  drawOneUp_m = true;
  previewIsPlaying_m = playPreview_b;
}