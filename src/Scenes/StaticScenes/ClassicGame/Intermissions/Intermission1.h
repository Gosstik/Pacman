#pragma once

#include "ClassicGameMusic.h"
#include "Globals.h"

class Intermission1 : public sf::Drawable {
 private:
  static constexpr float pacmanTimeNextFrame_m = 0.02f * mcsInSec;
  static constexpr float ghostTimeNextFrame_m = 0.15f * mcsInSec;
  static constexpr float pacman1Speed_m = -maxSpeed * 0.9f;
  static constexpr float pacman2Speed_m = maxSpeed * 0.9f;
  static constexpr float ghost1Speed_m = -maxSpeed * 0.95f;
  static constexpr float ghost2Speed_m = maxSpeed * 0.6f;
  static constexpr float posPixY_m = viewHeightPix_f / 2.0f;
  static constexpr float pacmanNormalScale_m = 1.0f;
  static constexpr float pacmanReverseScale_m = 3.0f;

  bool isPlaying_m = false;

  float accumTimePacmanAnim_m = 0;
  float accumTimeGhostAnim_m = 0;
  float accumTimeGeneral_m = 0;
  float intermissionDuration_m;

  int pacmanCurrFrame_m = 0;
  float pacman1PosPixX_m, pacman2PosPixX_m, pacman3PosPixX_m;
  bool pacmanReverseMove_m = false;
  float pacmanReverseTime_m;
  sf::Sprite pacman_m;

  int ghostCurrFrame_m = 0;
  int ghostCurrRectLeft_m = 0;
  float ghost1PosPixX_m, ghost2PosPixX_m, ghost3PosPixX_m;
  bool ghostReverseMove_m = false;
  float ghostReverseTime_m;
  sf::Sprite ghost_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  Intermission1(sf::Texture& texture, float leftRectBorderPixX,
                float rightRectBorderPixX, float intermissionDurationInMcs);
  void update(ClassicGameMusic& CGMusic, float dTime);
  void reload(ClassicGameMusic& CGMusic);
  bool ended() const;
};