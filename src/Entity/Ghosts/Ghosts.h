#pragma once

#include "Blinky.h"
#include "ClassicGameMusic.h"
#include "Clyde.h"
#include "Inky.h"
#include "Map.h"
#include "Pinky.h"
#include "Score.h"

class Ghosts {
  static const std::string imageGhostsFilename_m;
  static const std::string imageKillScoreFilename_m;

  sf::Image ghostsImg_m;
  sf::Texture ghostsTex_m;

  sf::Image killScoreImg_m;
  sf::Texture killScoreTex_m;
  sf::Sprite killScoreSpr_m;
  const std::vector<int> killScores_m{200, 400, 800, 1200};
  int killScoreIndex_m = 0;

  Blinky blinky_m;
  Pinky pinky_m;
  Inky inky_m;
  Clyde clyde_m;

  std::vector<Ghost*>
      cagePopulation_m;  // [0] - center, [1] - sprLeft, [2] - sprRight
  Ghost* releasingGhost_m = nullptr;

  GhostsTimes tm_m;
  GhostsStates st_m;
  GhostsMode pausedMode_m;
  GhostsMode currMode_m;
  const std::vector<int> lvlChangeSpeedRatio{1, 2, 5};
  const std::vector<float> normalSpeedRatio_m{0.75f, 0.85f, 0.95f};
  const std::vector<float> tunnelSpeedRatio_m{0.4f, 0.45f, 0.5f};
  const std::vector<float> frightSpeedRatio_m{0.5f, 0.55f, 0.6f};
  float normalSpeed_m = 0;
  float tunnelSpeed_m = 0;
  float frightSpeed_m = 0;

  const std::vector<int> leftCageCellLimitDotsVec_m{30, 0, 0};
  const std::vector<int> rightCageCellLimitDotsVec_m{90, 50, 0};
  int centralCageLimitDots_m;
  int leftCageCellLimitDots_m;
  int rightCageCellLimitDots_m;
  int dotsCounter_m = 0;

  void handleCage(float dTime);
  void setStartCagePopulation();
  void handleBigGumEffect(ClassicGameMusic& CGMusic, float dTime);
  void handleMoveToCageMusic(ClassicGameMusic& CGMusic) const;
  void handleMode(Map& map, Pacman& pacman, float dTime);
  template <typename G>
  void handlePacmanCollisionG(G& ghost, Map& map, Pacman& pacman, Score& score,
                              ClassicGameMusic& CGMusic,
                              bool& someGhostWasKilled_m,
                              bool& pauseBeforeKillAnimation);
  void updateSpeed(int level);
  void updateCageLimits(int level);

 public:
  explicit Ghosts(Map& map, int level);
  //    Ghosts(const Ghosts& other);
  //    Ghosts& operator=(const Ghosts& other);
  //    Ghosts(Ghosts&& other) noexcept;
  //    Ghosts& operator=(Ghosts&& other) noexcept;

  void update(MyWindow& window, Map& map, Pacman& pacman,
              ClassicGameMusic& CGMusic, int allGumsLeft, float dTime);
  void setBigGumEffect(Map& map, ClassicGameMusic& CGMusic);
  void handlePacmanCollision(Map& map, Pacman& pacman, Score& score,
                             ClassicGameMusic& CGMusic,
                             bool& someGhostWasKilled_m,
                             bool& pauseBeforeKillAnimation);

  void setScatterMode(Map& map, Pacman& pacman);
  void setChaseMode(Map& map, Pacman& pacman);

  void resetCageAccumTime();
  void pacmanAteBonus();

  void removeJustBeenKilled();

  void drawWithoutBigGumEffect(sf::RenderTarget& target) const;
  void drawWithBigGumEffect(sf::RenderTarget& target) const;
  void drawKillScore(sf::RenderTarget& target) const;

  bool smbIsMovingToCage() const;
  bool bigGumEffect() const;

  const sf::Texture* getGhostsTexture() const;
  const sf::Texture* getKillScoreTexture() const;

  /// change in case adding or deleting variables
  void reloadAfterPacmanDeath();
  void reloadNextLevel(int level);
  void reloadNewGame();
};

template <typename G>
void Ghosts::handlePacmanCollisionG(G& ghost, Map& map, Pacman& pacman,
                                    Score& score, ClassicGameMusic& CGMusic,
                                    bool& someGhostWasKilled_m,
                                    bool& pauseBeforeKillAnimation) {
  if (pacman.getHitboxBounds().intersects(ghost.getHitboxBounds()) &&
      ghost.isAlive()) {
    if (ghost.canBeEaten()) {
      ghost.kill(map, cagePopulation_m);
      CGMusic.eatGhostSound.play();
      score.add(killScores_m[killScoreIndex_m]);
      killScoreSpr_m.setTextureRect(
          {killScoreIndex_m * entitySize_i, 0, entitySize_i, entitySize_i});
      killScoreSpr_m.setPosition(ghost.getSpriteCenterPixX(),
                                 ghost.getSpriteCenterPixY());
      ++killScoreIndex_m;
      killScoreIndex_m %= 4;
      ghost.justBeenKilled_m = true;
      someGhostWasKilled_m = true;
    } else {
      CGMusic.stopAllMusic();
      pacman.kill();
      pauseBeforeKillAnimation = true;
    }
  }
}
// template<> void Ghosts::handlePacmanCollisionG(Blinky& ghost, Map& map,
// Pacman& pacman, Score& score,
//                                                ClassicGameMusic& CGMusic,
//                                                bool& someGhostWasKilled_m,
//                                                bool&
//                                                pauseBeforeKillAnimation);
// template<> void Ghosts::handlePacmanCollisionG(Pinky& ghost, Map& map,
// Pacman& pacman, Score& score,
//                                                ClassicGameMusic& CGMusic,
//                                                bool& someGhostWasKilled_m,
//                                                bool&
//                                                pauseBeforeKillAnimation);
// template<> void Ghosts::handlePacmanCollisionG(Inky& ghost, Map& map, Pacman&
// pacman, Score& score,
//                                                ClassicGameMusic& CGMusic,
//                                                bool& someGhostWasKilled_m,
//                                                bool&
//                                                pauseBeforeKillAnimation);
// template<> void Ghosts::handlePacmanCollisionG(Clyde& ghost, Map& map,
// Pacman& pacman, Score& score,
//                                                ClassicGameMusic& CGMusic,
//                                                bool& someGhostWasKilled_m,
//                                                bool&
//                                                pauseBeforeKillAnimation);