#include "Intermission3.h"

void Intermission3::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  target.draw(pacman_m);
  target.draw(ghost_m);
  target.draw(tentacle_m);
}

Intermission3::Intermission3(sf::Texture& texture, float leftRectBorderPixX,
                             float rightRectBorderPixX) {
  // initializing variables
  pacman1PosPixX_m = rightRectBorderPixX + entitySize_f / 2.0f;
  pacman2PosPixX_m = leftRectBorderPixX - entitySize_f / 2.0f;

  ghost1PosPixX_m = pacman1PosPixX_m + 4.0f * entitySize_f;
  ghost2PosPixX_m = pacman2PosPixX_m;

  tentacle1PosPixX_m = leftRectBorderPixX - tentacleWidth_m * 2;
  tentacle2PosPixX_m = rightRectBorderPixX + tentacleWidth_m;

  // creating sprites
  pacman_m.setTexture(texture);
  pacman_m.setTextureRect({0, 0, entitySize_i, entitySize_i});
  pacman_m.setOrigin(pacman_m.getLocalBounds().width / 2.0f,
                     pacman_m.getLocalBounds().height / 2.0f);
  pacman_m.setPosition(pacman1PosPixX_m, posPixY_m);
  pacman_m.setRotation(180);

  ghost_m.setTexture(texture);
  ghost_m.setTextureRect({0, entitySize_i, entitySize_i, entitySize_i});
  ghost_m.setOrigin(ghost_m.getLocalBounds().width / 2.0f,
                    ghost_m.getLocalBounds().height / 2.0f);
  ghost_m.setPosition(ghost1PosPixX_m, posPixY_m);

  tentacle_m.setTexture(texture);
  tentacle_m.setTextureRect({tentacleStartRectLeft_m, tentacleRectTop_m,
                             int(tentacleWidth_m), entitySize_i});
  tentacle_m.setOrigin(tentacle_m.getLocalBounds().width / 2.0f,
                       tentacle_m.getLocalBounds().height / 2.0f);
  tentacle_m.setPosition(tentacle1PosPixX_m, posPixY_m);
}

void Intermission3::update(ClassicGameMusic& CGMusic, float dTime) {
  if (pacman_m.getPosition().x > pacman2PosPixX_m)
    pacman_m.move(pacmanSpeed_m * dTime, 0);
  if (ghost_m.getPosition().x > ghost2PosPixX_m)
    ghost_m.move(ghostSpeed_m * dTime, 0);
  if (!tentacleIsMoving_m) {
    if (ghost_m.getPosition().x <= ghost2PosPixX_m) {
      tentacleIsMoving_m = true;
      tentacle_m.move(tentacleSpeed_m * dTime, 0);
    }
  } else if (tentacle_m.getPosition().x < tentacle2PosPixX_m)
    tentacle_m.move(tentacleSpeed_m * dTime, 0);
  else {
    CGMusic.intermissionSound.stop();
    isPlaying_m = false;
    return;
  }

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
        {ghostStartRectLeft_m + ghostCurrFrame_m * entitySize_i, ghostRectTop_m,
         entitySize_i, entitySize_i});
  }

  accumTimeTentacleAnim_m += dTime;
  if (accumTimeTentacleAnim_m >= tentacleTimeNextFrame_m) {
    while (accumTimeTentacleAnim_m >= tentacleTimeNextFrame_m) {
      accumTimeTentacleAnim_m -= tentacleTimeNextFrame_m;
      ++tentacleCurrFrame_m;
    }
    tentacleCurrFrame_m %= tentacleMaxFrameMove_m;
    tentacle_m.setTextureRect(
        {tentacleStartRectLeft_m + tentacleCurrFrame_m * int(tentacleWidth_m),
         tentacleRectTop_m, int(tentacleWidth_m), entitySize_i});
  }
}

void Intermission3::reload(ClassicGameMusic& CGMusic) {
  CGMusic.intermissionSound.setLoop(true);
  CGMusic.intermissionSound.play();

  pacman_m.setTextureRect({0, 0, entitySize_i, entitySize_i});
  pacman_m.setPosition(pacman1PosPixX_m, posPixY_m);

  ghost_m.setTextureRect(
      {ghostStartRectLeft_m, ghostRectTop_m, entitySize_i, entitySize_i});
  ghost_m.setPosition(ghost1PosPixX_m, posPixY_m);

  tentacle_m.setTextureRect({tentacleStartRectLeft_m, tentacleRectTop_m,
                             int(tentacleWidth_m), entitySize_i});
  tentacle_m.setPosition(tentacle1PosPixX_m, posPixY_m);

  accumTimePacmanAnim_m = accumTimeGhostAnim_m = accumTimeTentacleAnim_m = 0;
  pacmanCurrFrame_m = ghostCurrFrame_m = tentacleCurrFrame_m = 0;
  tentacleIsMoving_m = false;
  isPlaying_m = true;
}

bool Intermission3::ended() const { return !isPlaying_m; }