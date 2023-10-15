#pragma once

#include "Bonuses.h"
#include "Ghosts.h"
#include "MyText.h"
#include "Pacman.h"

class Preview : public sf::Drawable {
 private:
  enum State {
    start = 0,
    blinkyPic,
    shadow,
    blinkyText,
    pinkyPic,
    speedy,
    pinkyText,
    inkyPic,
    bashful,
    inkyText,
    clydePic,
    pokey,
    clydeText,
    gums,
    nickname,
    move
  };
  /// general constants
  static constexpr float textScale_m = 1.0f;
  static constexpr float constTimeShortPause_m = 0.5f * mcsInSec;
  static constexpr float constTimeLongPause_m = constTimeShortPause_m * 2.0f;
  static constexpr float constTimeGhostKill_m = 0.5f * mcsInSec;
  static constexpr float constTimeShowBigGums_m = 0.2 * mcsInSec;

  static constexpr float ghostNormalSpeed_m = 0.85f * maxSpeed;
  static constexpr float ghostFrightSpeed_m = 0.55f * maxSpeed;

  static constexpr int killScoreStartRectL_m = 0;
  static constexpr int killScoreStartRectT_m = 0;

  static constexpr int ptsRectL_m = 0;
  static constexpr int ptsRectT_m = 0;
  static constexpr int ptsWidth_m = cellSize_i * 3;
  static constexpr int ptsHeight_m = cellSize_i;

  static constexpr int copyrightRectL_m = 0;
  static constexpr int copyrightRectT_m = cellSize_i + cellSize_i / 8;

  /// position constants
  static constexpr float charAndNickPosX_m = viewWidthPix_f * 0.27f;
  static constexpr float charAndNickPosY_m = viewHeightPix_f * 0.15f;

  static constexpr float ghostPicturesPosX_m =
      charAndNickPosX_m - cellSize_f * 3.5f;
  static constexpr float characterPosX_m = charAndNickPosX_m;
  static constexpr float nicknamePosX_m =
      charAndNickPosX_m + cellSize_f * 11.0f;

  static constexpr float ghostPicturesStartPosY_m =
      charAndNickPosY_m + cellSize_f * 2.0f;
  static constexpr float descriptionsStartPosY_m =
      ghostPicturesStartPosY_m + cellSize_f / 2.0f;
  static constexpr float textOffset_m = cellSize_f * 3.0f;

  static constexpr float blinkyPicPosY_m = ghostPicturesStartPosY_m;
  static constexpr float blinkyTextPosY_m = descriptionsStartPosY_m;
  static constexpr float pinkyPicPosY_m =
      ghostPicturesStartPosY_m + textOffset_m;
  static constexpr float pinkyTextPosY_m =
      descriptionsStartPosY_m + textOffset_m;
  static constexpr float inkyPicPosY_m =
      ghostPicturesStartPosY_m + textOffset_m * 2;
  static constexpr float inkyTextPosY_m =
      descriptionsStartPosY_m + textOffset_m * 2;
  static constexpr float clydePicPosY_m =
      ghostPicturesStartPosY_m + textOffset_m * 3;
  static constexpr float clydeTextPosY_m =
      descriptionsStartPosY_m + textOffset_m * 3;

  static constexpr float entityPosY_m = clydeTextPosY_m + cellSize_f * 4.5f;
  static constexpr float pacmanStartPosX_m = viewWidthPix_f + entitySize_f;
  static constexpr float blinkyStartPosX_m =
      pacmanStartPosX_m + entitySize_f * 2;
  static constexpr float pinkyStartPosX_m =
      pacmanStartPosX_m + entitySize_f * 3;
  static constexpr float inkyStartPosX_m = pacmanStartPosX_m + entitySize_f * 4;
  static constexpr float clydeStartPosX_m =
      pacmanStartPosX_m + entitySize_f * 5;
  static constexpr float bigGumPosX_m = ghostPicturesPosX_m + cellSize_f;

  static constexpr float gumsPicPosX_m = viewWidthPix_f * 0.35f;
  static constexpr float numsPosX_m = gumsPicPosX_m + cellSize_f * 2.0f;
  static constexpr float ptsPosX_m = numsPosX_m + cellSize_f * 3.0f;
  static constexpr float gumPicPosY_m = entityPosY_m + cellSize_f * 4.0f;
  static constexpr float bigGumPicPosY_m = gumPicPosY_m + cellSize_f * 2.0f;

  static constexpr float copyrightPosX_m =
      viewWidthPix_f / 2.0f - cellSize_f * 6.5f;
  static constexpr float copyrightPosY_m = bigGumPicPosY_m + cellSize_f * 4.5f;
  static constexpr float authorPosX_m = copyrightPosX_m + cellSize_f * 2.0f;
  static constexpr float authorPosY_m = copyrightPosY_m;

  static constexpr float pressSpacePosX_m = viewWidthPix_f / 2.0f;
  static constexpr float pressSpacePosY_m = authorPosY_m + cellSize_f * 3.0f;

  /// drawing classes
  MyText CharAndNick_m;

  sf::Sprite blinkyPic_m;
  MyText shadow_m;
  MyText blinkyText_m;

  sf::Sprite pinkyPic_m;
  MyText speedy_m;
  MyText pinkyText_m;

  sf::Sprite inkyPic_m;
  MyText bashful_m;
  MyText inkyText_m;

  sf::Sprite clydePic_m;
  MyText pokey_m;
  MyText clydeText_m;

  Pacman pacman_m;
  Blinky blinky_m;
  Pinky pinky_m;
  Inky inky_m;
  Clyde clyde_m;
  Sprite1H bigGum_m;

  sf::Sprite killScoreSpr_m;

  sf::Image image_m;
  sf::Texture texture_m;

  sf::Sprite gumPic_m;
  MyText ten_m;
  sf::Sprite pts1_m;

  sf::Sprite bigGumPic_m;
  MyText fifty_m;
  sf::Sprite pts2_m;

  sf::Sprite copyright_m;
  MyText author_m;

  MyText pressSpace_m;

  /// additional
  sf::Sound& munch_m;
  State state_m = start;

  int killScoreIndex_m = 0;

  float accumTimePause_m = 0;
  float accumTimeGhostKill_m = 0;
  float accumTimeShowBigGums_m = 0;

  bool someGhostWasKilled_m = false;
  bool bigGumWasEaten_m = false;
  bool showBigGums_m = true;

  /// new
  bool isPlaying_m = true;
  bool startTurnOff_m = false;
  bool firstIteration_m = true;
  bool showPressSpace_m = false;
  float accumTimePressSpace_m = 0;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  void drawGhostsWithoutBigGumEffect(sf::RenderTarget& target) const;
  void drawGhostsWithBigGumEffect(sf::RenderTarget& target) const;

  void handleBigGums(float dTime);
  template <typename G>
  void handleCollisionG(G& ghost);

 public:
  Preview(Pacman& pacman, Ghosts& ghosts, Bonuses& bonuses, Music& music);
  void update(float dTime);
  void turnOff();
  void reload();
  void reloadNewGame();
  bool isPlaying() const;
};

template <typename G>
void Preview::handleCollisionG(G& ghost) {
  if (pacman_m.getHitboxBounds().intersects(ghost.getHitboxBounds()) &&
      ghost.isAlive()) {
    killScoreSpr_m.setTextureRect(
        {killScoreStartRectL_m + killScoreIndex_m * entitySize_i,
         killScoreStartRectT_m, entitySize_i, entitySize_i});
    killScoreSpr_m.setPosition(ghost.getSpriteCenterPixX(),
                               ghost.getSpriteCenterPixY());
    ++killScoreIndex_m;
    ghost.previewKill();
    someGhostWasKilled_m = true;
  }
}