#pragma once

#include "ClassicGameMusic.h"
#include "Globals.h"

class Intermission3 : public sf::Drawable {
 private:
  static constexpr float pacmanTimeNextFrame_m = 0.02f * mcsInSec;
  static constexpr float ghostTimeNextFrame_m = 0.15f * mcsInSec;
  static constexpr float tentacleTimeNextFrame_m = 0.1f * mcsInSec;
  static constexpr float posPixY_m = viewHeightPix_f / 2.0f;
  static constexpr float pacmanSpeed_m = -maxSpeed * 0.9f;
  static constexpr float ghostSpeed_m = -maxSpeed * 0.95f;
  static constexpr int ghostStartRectLeft_m = 4 * entitySize_i;
  static constexpr int ghostRectTop_m = entitySize_i;
  static constexpr float tentacleSpeed_m = maxSpeed * 0.8f;
  static constexpr int tentacleStartRectLeft_m = 5 * entitySize_i;
  static constexpr int tentacleRectTop_m = 2 * entitySize_i;
  static constexpr float tentacleWidth_m = 3 * cellSize_f;

  bool isPlaying_m = false;

  float accumTimePacmanAnim_m = 0;
  float accumTimeGhostAnim_m = 0;
  float accumTimeTentacleAnim_m = 0;

  int pacmanCurrFrame_m = 0;
  float pacman1PosPixX_m, pacman2PosPixX_m;
  sf::Sprite pacman_m;

  int ghostCurrFrame_m = 0;
  float ghost1PosPixX_m, ghost2PosPixX_m;
  sf::Sprite ghost_m;

  int tentacleCurrFrame_m = 0;
  int tentacleMaxFrameMove_m = 2;
  float tentacle1PosPixX_m, tentacle2PosPixX_m;
  bool tentacleIsMoving_m = false;
  sf::Sprite tentacle_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  Intermission3(sf::Texture& texture, float leftRectBorderPixX,
                float rightRectBorderPixX);
  void update(ClassicGameMusic& CGMusic, float dTime);
  void reload(ClassicGameMusic& CGMusic);
  bool ended() const;
};