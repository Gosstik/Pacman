#pragma once

#include <list>

#include "Bonuses.h"
#include "ClassicGameMusic.h"
#include "MyText.h"
#include "Pacman.h"
#include "Score.h"

class ClassicGameLabels : public sf::Drawable {
  static constexpr float textScale_m = 1.0f;
  static constexpr int livesRectLeft_m = 3 * entitySize_i;
  static constexpr int livesRectTop_m = 0;
  static constexpr float constTimeDrawOneUp_m = 0.25f * mcsInSec;

  /// label "1up"
  static constexpr float oneUpPosX_m = viewWidthPix_f * 0.25f;
  static constexpr float oneUpPosY_m = cellSize_f / 2.0f;
  // initialised by constructor
  MyText oneUp_m;

  /// label "scoreNum"
  static constexpr float scoreNumPosX_m = oneUpPosX_m;
  static constexpr float scoreNumPosY_m =
      oneUpPosY_m + cellSize_f * textScale_m * 1.2f;
  // initialised by constructor
  MyText scoreNum_m;

  /// label "high score"
  static constexpr float highScorePosX_m = viewWidthPix_f * 0.5f;
  static constexpr float highScorePosY_m = oneUpPosY_m;
  // initialised by constructor
  MyText highScore_m;

  /// label "highScoreNum"
  static constexpr float highScoreNumPosX_m = highScorePosX_m;
  static constexpr float highScoreNumPosY_m =
      highScorePosY_m + cellSize_f * textScale_m * 1.2f;
  // initialised by constructor
  MyText highScoreNum_m;

  /// label "level"
  static constexpr float levelPosX_m = viewWidthPix_f * 0.75f;
  static constexpr float levelPosY_m = oneUpPosY_m;
  // initialised by constructor
  MyText level_m;

  /// label "levelNum"
  static constexpr float levelNumPosX_m = levelPosX_m;
  static constexpr float levelNumPosY_m =
      levelPosY_m + cellSize_f * textScale_m * 1.2f;
  // initialised by constructor
  MyText levelNum_m;

  /// label "ready!"
  static constexpr float readyPosX_m = cageCenterPixX;
  static constexpr float readyPosY_m = cageCenterPixY + cellSize_f * 3.0f;
  sf::Color readyColor_m = sf::Color(255, 255, 0);
  // initialised by constructor
  MyText ready_m;

  /// label "countdown"
  int CDNum_m = 0;
  static constexpr float countdownNumScale_m = 2.0f;
  static constexpr float countdownNumPosX_m = cageCenterPixX;
  static constexpr float countdownNumPosY_m =
      (float(mapOffsetCellY_i) + 3.5f) * cellSize_f;
  sf::Color countdownNumColor_m = readyColor_m;
  // initialised by constructor
  MyText countdownNum_m;

  /// label "game over"
  static constexpr float gameOverPosX_m = cageCenterPixX;
  static constexpr float gameOverPosY_m = readyPosY_m;
  sf::Color gameOverColor_m = sf::Color(255, 0, 0);
  // initialised by constructor
  MyText gameOver_m;

  /// images of getExtraLives
  static constexpr float livesStartPosX_m =
      (float(mapOffsetCellX_i) + 2.5f) * cellSize_f;
  static constexpr float livesStartPosY_m =
      (float(mapOffsetCellY_i + mapHeightCell + 1)) * cellSize_f;
  static constexpr float livesOffsetX = entitySize_f;
  sf::Sprite livesSpr_m;
  std::list<sf::Sprite> livesLst_m;

  /// images of eaten fruits
  static constexpr float firstFruitPosX =
      float(mapOffsetCellX_i + mapWidthCell - 1) * cellSize_f;
  static constexpr float firstFruitPosY = livesStartPosY_m;
  static constexpr size_t maxFruitLabels_m = 7;
  sf::Sprite fruitSpr_m;
  std::list<sf::Sprite> fruitsLst_m;

  /// additional
  float accumTime_m = 0;
  float accumTimeChangeMapColor_m = 0;
  float accumTimeDrawOneUp_m = 0;

  bool startLevelAnimation_m = false;
  bool gameOverAnimation_m = false;
  bool continueLevelAnimation_m = false;
  bool endLevelAnimation_m = false;
  bool drawOneUp_m = true;
  bool previewIsPlaying_m = true;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void handleCountdown(bool& state, float dTime);
  void reloadFruitsLabelNextLevel(Bonuses& bonuses);
  void reloadCountdown(int seconds);

 public:
  bool intermissionIsPlaying_m = false;

  ClassicGameLabels(Pacman& pacman, Score& score, Bonuses& bonuses, int level);
  void update(Map& map, Score& score, ClassicGameMusic& CGMusic, float dTime);
  void setGameOverAnimation();
  void setContinueLevelAnimation(ClassicGameMusic& CGMusic);
  void setEndLevelAnimation(Map& map);
  void continueStartLevelAnimation(ClassicGameMusic& CGMusic);
  void turnOffPreview();
  bool startLevelAnimation() const;
  bool gameOverAnimation() const;
  bool continueLevelAnimation() const;
  bool endLevelAnimation() const;

  void addLife();
  void updateUpLabel(float dTime);

  void reloadAfterPacmanDeath(ClassicGameMusic& CGMusic);
  void reloadNextLevel(Bonuses& bonuses, ClassicGameMusic& CGMusic, int level);
  void reloadNewGame(Pacman& pacman, Bonuses& bonuses, Score& score);
};