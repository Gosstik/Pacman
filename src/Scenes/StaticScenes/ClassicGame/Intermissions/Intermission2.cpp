#include "Intermission2.h"

void Intermission2::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  target.draw(pacman_m);
  if (remainsShow_m)
    target.draw(remains_m);
  else
    target.draw(obstacle_m);
  target.draw(ghost_m);
}

Intermission2::Intermission2(sf::Texture& texture, float leftRectBorderPixX,
                             float rightRectBorderPixX) {
  // initializing variables
  obstaclePosPixX_m = viewWidthPix_f / 2.0f;

  pacman1PosPixX_m = rightRectBorderPixX + entitySize_f / 2.0f;
  pacman2PosPixX_m = leftRectBorderPixX - entitySize_f / 2.0f;

  ghost1PosPixX_m = pacman1PosPixX_m;
  ghost2PosPixX_m = obstaclePosPixX_m;  // - 6.0f * cellSize_f / 8.0f;
  ghost3PosPixX_m = ghost2PosPixX_m - 10.0f * cellSize_f / 8.0f;
  ghost4PosPixX_m = ghost3PosPixX_m - 14.0f + cellSize_f / 8.0f;

  remainsPosPix_m = obstaclePosPixX_m;

  // creating sprites
  pacman_m.setTexture(texture);
  pacman_m.setTextureRect({0, 0, entitySize_i, entitySize_i});
  pacman_m.setOrigin(pacman_m.getLocalBounds().width / 2.0f,
                     pacman_m.getLocalBounds().height / 2.0f);
  pacman_m.setPosition(pacman1PosPixX_m, posPixY_m);
  pacman_m.setRotation(180);

  obstacle_m.setTexture(texture);
  obstacle_m.setTextureRect(
      {0, 2 * entitySize_i, entitySize_i, entitySize_i});  // remove ???
  obstacle_m.setOrigin(obstacle_m.getLocalBounds().width / 2.0f,
                       obstacle_m.getLocalBounds().height / 2.0f);
  obstacle_m.setPosition(obstaclePosPixX_m, posPixY_m);

  ghost_m.setTexture(texture);
  ghost_m.setTextureRect(
      {0, entitySize_i, entitySize_i, entitySize_i});  // remove ???
  ghost_m.setOrigin(ghost_m.getLocalBounds().width / 2.0f,
                    ghost_m.getLocalBounds().height / 2.0f);
  ghost_m.setPosition(ghost1PosPixX_m, posPixY_m);

  remains_m.setTexture(texture);
  remains_m.setTextureRect({remainsStartRectLeft_m, remainsRectTop_m,
                            entitySize_i, entitySize_i});  // remove ???
  remains_m.setOrigin(remains_m.getLocalBounds().width / 2.0f,
                      remains_m.getLocalBounds().height / 2.0f);
  remains_m.setPosition(remainsPosPix_m, posPixY_m);
}

void Intermission2::update(ClassicGameMusic& CGMusic, float dTime) {
  if (pacman_m.getPosition().x > pacman2PosPixX_m)
    pacman_m.move(pacmanSpeed_m * dTime, 0);

  if (ghostState_m == stand) {
    if (pacman_m.getPosition().x < obstaclePosPixX_m) {
      ghostState_m = move1;
      ghost_m.move(ghost1Speed_m * dTime, 0);
    }
  } else if (ghostState_m == move1) {
    if (ghost_m.getPosition().x - ghost1Speed_m * dTime <= ghost2PosPixX_m) {
      ghost_m.setPosition(ghost2PosPixX_m, posPixY_m);
      ghostState_m = move2;
      ghost_m.move(ghost2Speed_m * dTime, 0);
      remainsShow_m = true;
    } else
      ghost_m.move(ghost1Speed_m * dTime, 0);
  } else if (ghostState_m == move2) {
    if (ghost_m.getPosition().x - ghost2Speed_m * dTime <= ghost3PosPixX_m) {
      ghost_m.setPosition(ghost3PosPixX_m, posPixY_m);
      ghostState_m = stopped;
      ghost_m.setTextureRect(
          {entitySize_i, entitySize_i, entitySize_i, entitySize_i});
      accumTimeGhostAnim_m = 0;
      ghostCurrFrame_m = 0;
      remainsCurrFrame_m = 2;
      remains_m.setTextureRect(
          {remainsStartRectLeft_m + remainsCurrFrame_m * entitySize_i,
           remainsRectTop_m, entitySize_i, entitySize_i});
      return;
    } else
      ghost_m.move(ghost2Speed_m * dTime, 0);
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
  if (ghostState_m == move1) {
    if (accumTimeGhostAnim_m >= ghostTime1NextFrame_m) {
      while (accumTimeGhostAnim_m >= ghostTime1NextFrame_m) {
        accumTimeGhostAnim_m -= ghostTime1NextFrame_m;
        ++ghostCurrFrame_m;
      }
      ghostCurrFrame_m %= ghostMaxFrameMove;
      ghost_m.setTextureRect(
          {ghostCurrRectLeft_m + ghostCurrFrame_m * entitySize_i, entitySize_i,
           entitySize_i, entitySize_i});
    }
  }
  if (ghostState_m == move2) {
    if (accumTimeGhostAnim_m >= ghostTime2NextFrame_m) {
      while (accumTimeGhostAnim_m >= ghostTime2NextFrame_m) {
        accumTimeGhostAnim_m -= ghostTime2NextFrame_m;
        ++ghostCurrFrame_m;
      }
      ghostCurrFrame_m %= ghostMaxFrameMove;
      ghost_m.setTextureRect(
          {ghostCurrRectLeft_m + ghostCurrFrame_m * entitySize_i, entitySize_i,
           entitySize_i, entitySize_i});
    }

    if (remainsCurrFrame_m == 0) {
      if (ghost_m.getPosition().x <=
          ghost2PosPixX_m - 6.0f * cellSize_f / 8.0f) {
        remainsCurrFrame_m = 1;
        remains_m.setTextureRect(
            {remainsStartRectLeft_m + remainsCurrFrame_m * entitySize_i,
             remainsRectTop_m, entitySize_i, entitySize_i});
      }
    }
  } else if (ghostState_m == stopped) {
    if (accumTimeGhostAnim_m >= ghostTimeLastMoves_m) {
      while (accumTimeGhostAnim_m >= ghostTimeLastMoves_m) {
        accumTimeGhostAnim_m -= ghostTimeLastMoves_m;
        if (remainsCurrFrame_m == 2) {
          ghost_m.setPosition(ghost4PosPixX_m, posPixY_m);
          ghostCurrRectLeft_m = 6 * entitySize_i;
          ghost_m.setTextureRect(
              {ghostCurrRectLeft_m, entitySize_i, entitySize_i, entitySize_i});
          remainsCurrFrame_m = 3;
          remains_m.setTextureRect(
              {remainsStartRectLeft_m + remainsCurrFrame_m * entitySize_i,
               remainsRectTop_m, entitySize_i, entitySize_i});
        } else if (ghostCurrRectLeft_m == 6 * entitySize_i) {
          ghostCurrRectLeft_m = 7 * entitySize_i;
          ghost_m.setTextureRect(
              {ghostCurrRectLeft_m, entitySize_i, entitySize_i, entitySize_i});
        } else if (ghostCurrRectLeft_m == 7 * entitySize_i) {
          isPlaying_m = false;
          CGMusic.intermissionSound.stop();
        }
      }
    }
  }
}

void Intermission2::reload(ClassicGameMusic& CGMusic) {
  CGMusic.intermissionSound.setLoop(true);
  CGMusic.intermissionSound.play();

  pacman_m.setTextureRect({0, 0, entitySize_i, entitySize_i});
  pacman_m.setPosition(pacman1PosPixX_m, posPixY_m);

  ghost_m.setTextureRect({0, entitySize_i, entitySize_i, entitySize_i});
  ghost_m.setPosition(ghost1PosPixX_m, posPixY_m);

  // remains
  remains_m.setTextureRect(
      {remainsStartRectLeft_m, remainsRectTop_m, entitySize_i, entitySize_i});

  isPlaying_m = true;

  accumTimePacmanAnim_m = accumTimeGhostAnim_m = 0;
  pacmanCurrFrame_m = ghostCurrFrame_m = 0;

  ghostState_m = stand;
  ghostCurrRectLeft_m = 0;

  remainsCurrFrame_m = 0;
  remainsShow_m = false;
}

bool Intermission2::ended() const { return !isPlaying_m; }