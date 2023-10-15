#pragma once

#include <vector>

#include "Entity.h"

class Pacman : public Entity {
  static const std::string imageFilename_m;

  static constexpr int pacmanStartPosMapX =
      16;  // 3; // 16; // actually pacman is located between two cells
  static constexpr int pacmanStartPosMapY = 23;  // 3; // 23;
  static constexpr float pacmanStartPosPixX =
      (pacmanStartPosMapX + mapOffsetCellX_i) *
      cellSize_i;  //(pacmanStartPosMapX + mapOffsetCellX_i + 0.5f) *
                   //cellSize_i;
  static constexpr float pacmanStartPosPixY =
      (pacmanStartPosMapY + mapOffsetCellY_i + 0.5f) * cellSize_i;
  static constexpr int pacmanStartTexRectL = 0;
  static constexpr int pacmanStartTexRectT = 0;
  static constexpr float pacmanTimeNextFrameKill = 100000.0f;
  static constexpr int maxFrameKill_m = 13;

  static constexpr float dotsSpeedRatio_m = 0.87f;  // make const
  static const std::vector<int> lvlChangeSpeedRatio_m;
  static const std::vector<float> normalSpeedRatio_m;
  static const std::vector<float> frightSpeedRatio_m;

  sf::Image image_m;
  sf::Texture texture_m;

  float constTimeNextFrame_m;
  bool killFrames_m = false;

  bool isKilled_m = false;
  bool killAnimationEnded_m = false;
  int extraLives_m = pacmanStartNumOfExtraLives;

  float coveredDistOneIter_m = 0;

  float normalSpeed_m;
  float normalDotsSpeed_m;
  float frightSpeed_m;
  float frightDotsSpeed_m;

  bool moveDiagonal_m = false;

  void move(Map& map, float dTime);
  void handleWallCollision(Map& map, float dTime, bool isNextDir);
  void animation(float dTime);
  void killAnimation(ClassicGameMusic& CGMusic, float dTime);
  void updateSpeed(int level);
  void calculateTimeConstants();
  void moveDiagonal(float dTime);
  void standardMove();

 public:
  explicit Pacman(int level = 1);
  ~Pacman() override = default;
  void updateDirection(sf::Event& event);
  void update(MyWindow& window, Map& map, ClassicGameMusic& CGMusic,
              float dTime);
  void kill();
  void setEndLevelAnimation();
  int getExtraLives() const;
  bool isKilled() const;
  bool killAnimationEnded() const;
  void addExtraLife();
  float getCoveredDistOneIter() const;
  void setNormalSpeed(bool frightMode);
  void setDotsSpeed(bool frightMode);

  /// preview functions
  void previewSetStart(float posX, float posY);
  void previewUpdate(float dTime);
  void previewChangeBehaviour();

  /// change in case adding or deleting variables
  void reloadAfterDeath() override;
  void reloadNextLevel(int level);
  void reloadNewGame();
};