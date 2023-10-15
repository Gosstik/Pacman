#pragma once

#include <fstream>

#include "MyText.h"

class Score {
  const std::string saveFilename_m = "../data/Score.info";
  int score_m = 0;
  int highScore_m = 0;
  int highScoreInFile_m = 0;
  int extraLifeScore_m = 0;

  void readData();

 public:
  Score();
  ~Score();
  void add(int val);
  void takeRemOfExtraLifeScore();

  [[nodiscard]] int getScoreNum() const;
  [[nodiscard]] int getHighScoreNum() const;
  [[nodiscard]] int getExtraLifeScore() const;

  /// change in case adding or deleting variables
  void reloadNewGame();
  void saveData();
};