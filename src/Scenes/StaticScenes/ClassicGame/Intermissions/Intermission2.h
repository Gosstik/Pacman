#pragma once

#include "ClassicGameMusic.h"
#include "Globals.h"

class Intermission2 : public sf::Drawable {
 private:
  static constexpr float pacmanTimeNextFrame_m = 0.02f * mcsInSec;
  static constexpr float ghostTime1NextFrame_m = 0.15f * mcsInSec;
  static constexpr float ghostTime2NextFrame_m = 0.21f * mcsInSec;
  static constexpr float ghostTimeLastMoves_m = 1.3f * mcsInSec;
  static constexpr float posPixY_m = viewHeightPix_f / 2.0f;
  static constexpr float pacmanSpeed_m = -maxSpeed * 0.9f;
  static constexpr float ghost1Speed_m = -maxSpeed * 1.05f;
  static constexpr float ghost2Speed_m = -maxSpeed * 0.1f;
  static constexpr int remainsStartRectLeft_m = entitySize_i;
  static constexpr int remainsRectTop_m = 2 * entitySize_i;

  enum GhostState { stand, move1, move2, stopped };

  bool isPlaying_m = false;

  float accumTimePacmanAnim_m = 0;
  float accumTimeGhostAnim_m = 0;

  int pacmanCurrFrame_m = 0;
  float pacman1PosPixX_m, pacman2PosPixX_m;
  sf::Sprite pacman_m;

  float obstaclePosPixX_m;
  sf::Sprite obstacle_m;

  int ghostCurrFrame_m = 0;
  GhostState ghostState_m = stand;
  int ghostCurrRectLeft_m = 0;
  float ghost1PosPixX_m, ghost2PosPixX_m, ghost3PosPixX_m, ghost4PosPixX_m;
  sf::Sprite ghost_m;

  int remainsCurrFrame_m = 0;
  bool remainsShow_m = false;
  float remainsPosPix_m;
  sf::Sprite remains_m;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 public:
  Intermission2(sf::Texture& texture, float leftRectBorderPixX,
                float rightRectBorderPixX);
  void update(ClassicGameMusic& CGMusic, float dTime);
  void reload(ClassicGameMusic& CGMusic);
  bool ended() const;
};