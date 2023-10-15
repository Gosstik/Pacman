#include "Score.h"

Score::Score() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::in);
  if (!gameFirstOpen) readData();
}

Score::~Score() { saveData(); }

void Score::add(int val) {
  score_m += val;
  extraLifeScore_m += val;
  if (highScore_m < score_m) highScore_m = score_m;
}

void Score::takeRemOfExtraLifeScore() { extraLifeScore_m %= scoreForExtraLife; }

int Score::getScoreNum() const { return score_m; }

int Score::getHighScoreNum() const { return highScore_m; }

int Score::getExtraLifeScore() const { return extraLifeScore_m; }

void Score::reloadNewGame() {
  score_m = 0;
  extraLifeScore_m = 0;
  saveData();
}

void Score::saveData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::out |
                                          std::fstream::trunc);
  if (highScore_m > highScoreInFile_m) highScoreInFile_m = highScore_m;
  stream.write(reinterpret_cast<char*>(&highScoreInFile_m),
               sizeof(highScoreInFile_m));
  stream.close();
}

void Score::readData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::in);
  stream.read(reinterpret_cast<char*>(&highScore_m), sizeof(highScore_m));
  stream.close();
}