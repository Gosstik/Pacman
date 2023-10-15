#include "Intermission1.h"

void Intermission1::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  target.draw(pacman_m);
  target.draw(ghost_m);
}

Intermission1::Intermission1(sf::Texture& texture, float leftRectBorderPixX,
                             float rightRectBorderPixX,
                             float intermissionDurationInMcs)
    : intermissionDuration_m(intermissionDurationInMcs) {
  // initializing variables
  pacman1PosPixX_m = rightRectBorderPixX + entitySize_f / 2.0f;
  pacman2PosPixX_m =
      leftRectBorderPixX - entitySize_f * pacmanReverseScale_m / 2.0f;
  pacman3PosPixX_m =
      rightRectBorderPixX + entitySize_f * pacmanReverseScale_m / 2.0f;

  ghost1PosPixX_m = pacman1PosPixX_m + 1.6f * entitySize_f;
  ghost2PosPixX_m = leftRectBorderPixX - entitySize_f / 2.0f;
  ghost3PosPixX_m = rightRectBorderPixX + entitySize_f * 2.5f;

  pacmanReverseTime_m = intermissionDurationInMcs -
                        (pacman3PosPixX_m - pacman2PosPixX_m) / pacman2Speed_m;
  ghostReverseTime_m = intermissionDurationInMcs -
                       (ghost3PosPixX_m - ghost2PosPixX_m) / ghost2Speed_m;

  // creating sprites
  pacman_m.setTexture(texture);
  pacman_m.setTextureRect({0, 0, entitySize_i, entitySize_i});
  pacman_m.setOrigin(pacman_m.getLocalBounds().width / 2.0f,
                     pacman_m.getLocalBounds().height / 2.0f);
  pacman_m.setPosition(pacman1PosPixX_m, posPixY_m);

  ghost_m.setTexture(texture);
  ghost_m.setTextureRect({0, entitySize_i, entitySize_i, entitySize_i});
  ghost_m.setOrigin(ghost_m.getLocalBounds().width / 2.0f,
                    ghost_m.getLocalBounds().height / 2.0f);
  ghost_m.setPosition(ghost1PosPixX_m, posPixY_m);
}

void Intermission1::update(ClassicGameMusic& CGMusic, float dTime) {
  accumTimeGeneral_m += dTime;
  // first condition is redundant
  if (accumTimeGeneral_m >= intermissionDuration_m &&
      CGMusic.intermissionSound.getStatus() != sf::SoundSource::Playing) {
    isPlaying_m = false;
    return;
  }
  if (accumTimeGeneral_m >= intermissionDuration_m / 2.0f) {
    if (CGMusic.intermissionSound.getLoop() &&
        CGMusic.intermissionSound.getPlayingOffset().asSeconds() <
            3) {  // dangerous place
      CGMusic.intermissionSound.setLoop(false);
    }
  }

  if (!pacmanReverseMove_m) {
    if (accumTimeGeneral_m >= pacmanReverseTime_m) {
      pacmanReverseMove_m = true;
      pacman_m.setScale(pacmanReverseScale_m, pacmanReverseScale_m);
      pacman_m.setRotation(0);
      pacman_m.move(pacman2Speed_m * dTime, 0);
      pacman_m.setTextureRect(
          {pacmanCurrFrame_m * entitySize_i, 0, entitySize_i, entitySize_i});
    } else if (pacman_m.getPosition().x > pacman2PosPixX_m)
      pacman_m.move(pacman1Speed_m * dTime, 0);
  } else if (pacman_m.getPosition().x < pacman3PosPixX_m)
    pacman_m.move(pacman2Speed_m * dTime, 0);

  if (!ghostReverseMove_m) {
    if (accumTimeGeneral_m >= ghostReverseTime_m) {
      ghostReverseMove_m = true;
      ghostCurrRectLeft_m = entitySize_i * ghostMaxFrameMove;
      ghost_m.move(ghost2Speed_m * dTime, 0);
      ghost_m.setTextureRect(
          {ghostCurrRectLeft_m + ghostCurrFrame_m * entitySize_i, entitySize_i,
           entitySize_i, entitySize_i});
    } else if (ghost_m.getPosition().x > ghost2PosPixX_m)
      ghost_m.move(ghost1Speed_m * dTime, 0);
  } else if (ghost_m.getPosition().x < ghost3PosPixX_m)
    ghost_m.move(ghost2Speed_m * dTime, 0);

  // animation
  accumTimePacmanAnim_m += dTime;
  if (accumTimePacmanAnim_m >= pacmanTimeNextFrame_m) {
    while (accumTimePacmanAnim_m >= pacmanTimeNextFrame_m) {
      accumTimePacmanAnim_m -= pacmanTimeNextFrame_m;
      ++pacmanCurrFrame_m;
    }
    pacmanCurrFrame_m %= pacmanMaxFrameMove;
    pacman_m.setTextureRect(
        {pacmanCurrFrame_m * entitySize_i, 0, entitySize_i, entitySize_i});
  }

  accumTimeGhostAnim_m += dTime;
  if (accumTimeGhostAnim_m >= ghostTimeNextFrame_m) {
    while (accumTimeGhostAnim_m >= ghostTimeNextFrame_m) {
      accumTimeGhostAnim_m -= ghostTimeNextFrame_m;
      ++ghostCurrFrame_m;
    }
    ghostCurrFrame_m %= ghostMaxFrameMove;
    ghost_m.setTextureRect(
        {ghostCurrRectLeft_m + ghostCurrFrame_m * entitySize_i, entitySize_i,
         entitySize_i, entitySize_i});
  }
}

void Intermission1::reload(ClassicGameMusic& CGMusic) {
  CGMusic.intermissionSound.setLoop(true);
  CGMusic.intermissionSound.play();

  pacman_m.setTextureRect({0, 0, entitySize_i, entitySize_i});
  pacman_m.setRotation(180);
  pacman_m.setScale(pacmanNormalScale_m, pacmanNormalScale_m);
  pacman_m.setPosition(pacman1PosPixX_m, posPixY_m);

  ghost_m.setTextureRect({0, entitySize_i, entitySize_i, entitySize_i});
  ghost_m.setPosition(ghost1PosPixX_m, posPixY_m);

  accumTimePacmanAnim_m = 0;
  accumTimeGhostAnim_m = 0;
  accumTimeGeneral_m = 0;

  pacmanCurrFrame_m = ghostCurrFrame_m = 0;
  ghostCurrRectLeft_m = 0;

  pacmanReverseMove_m = ghostReverseMove_m = false;
  isPlaying_m = true;
}

bool Intermission1::ended() const { return !isPlaying_m; }