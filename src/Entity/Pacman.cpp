#include "Pacman.h"

const std::vector<int> Pacman::lvlChangeSpeedRatio_m{1, 2, 5, 21};
const std::vector<float> Pacman::normalSpeedRatio_m{0.8f, 0.9f, 1.0f, 0.9f};
const std::vector<float> Pacman::frightSpeedRatio_m{0.9f, 0.95f, 1.0f, 1.0f};

Pacman::Pacman(int level)
    : Entity(texture_m, entityHbWidth, pacmanStartPosMapX, pacmanStartPosMapY,
             pacmanStartPosPixX, pacmanStartPosPixY, pacmanStartTexRectL,
             pacmanStartTexRectT) {
  image_m.loadFromFile(imageFilename_m);
  image_m.createMaskFromColor(sf::Color::Black);
  texture_m.loadFromImage(image_m);
  currDir_m = nextDir_m = Stop;

  updateSpeed(level);
  /// depend on speed
  calculateTimeConstants();
}

void Pacman::updateSpeed(int level) {
  size_t size = lvlChangeSpeedRatio_m.size();
  for (size_t i = 0; i < size; ++i)
    if ((i == size - 1) || level < lvlChangeSpeedRatio_m[i + 1]) {
      normalSpeed_m = currSpeed_m = maxSpeed * normalSpeedRatio_m[i];
      normalDotsSpeed_m = normalSpeed_m * dotsSpeedRatio_m;
      frightSpeed_m = maxSpeed * frightSpeedRatio_m[i];
      frightDotsSpeed_m = frightSpeed_m * dotsSpeedRatio_m;
      break;
    }
}

void Pacman::calculateTimeConstants() {
  constTimeNextFrame_m =
      cellSize_f / normalDotsSpeed_m / float(pacmanMaxFrameMove);
}

void Pacman::update(MyWindow& window, Map& map, ClassicGameMusic& CGMusic,
                    float dTime) {
  if (isKilled_m)
    killAnimation(CGMusic, dTime);
  else {
    move(map, dTime);
    animation(dTime);
  }
}

void Pacman::standardMove() {
  sprite2H_m.move(dx_m, dy_m);
  spriteCenterPixX_m += dx_m;
  spriteCenterPixY_m += dy_m;
  updateVariables();
  coveredDistOneIter_m = (dx_m > 0) ? abs(dx_m) : abs(dy_m);
}

void Pacman::moveDiagonal(float dTime) {
  float moveDist = currSpeed_m * dTime;
  if (currDir_m == Up || currDir_m == Down) {
    float offset = abs(cellCenterPixY_m - spriteCenterPixY_m);
    dx_m = float(nextDir_m - 2) * moveDist;
    if (offset > moveDist)
      dy_m = float(currDir_m - 1) * moveDist;
    else {
      dy_m = float(currDir_m - 1) * offset;
      moveDiagonal_m = false;
      hasTurned_m = true;
      currDir_m = nextDir_m;
    }
  } else {
    float offset = abs(cellCenterPixX_m - spriteCenterPixX_m);
    dy_m = float(nextDir_m - 1) * moveDist;
    if (offset > moveDist)
      dx_m = float(currDir_m - 2) * moveDist;
    else {
      dx_m = float(currDir_m - 2) * offset;
      moveDiagonal_m = false;
      hasTurned_m = true;
      currDir_m = nextDir_m;
    }
  }
  standardMove();
}

void Pacman::move(Map& map, float dTime) {
  if (moveDiagonal_m) {
    moveDiagonal(dTime);
  } else {
    float moveDist = dTime * currSpeed_m;
    handlePortal(moveDist);
    if (currDir_m != nextDir_m) {
      if (currDir_m != static_cast<Dir>((nextDir_m + 2) % 4)) {
        if ((nextDir_m == Right &&
             map.getCellState(cellCenterMapX_m + 1, cellCenterMapY_m) != 'W') ||
            (nextDir_m == Left &&
             map.getCellState(cellCenterMapX_m - 1, cellCenterMapY_m) != 'W')) {
          if (cellCenterPixY_m == spriteCenterPixY_m) {
            dx_m = float(nextDir_m - 2) * moveDist;
            dy_m = 0;
            standardMove();
            currDir_m = nextDir_m;
          } else {
            moveDiagonal_m = true;
            currDir_m = (cellCenterPixY_m - spriteCenterPixY_m > 0) ? Down : Up;
            moveDiagonal(dTime);
          }
          hasTurned_m = true;
        } else if ((nextDir_m == Up &&
                    map.getCellState(cellCenterMapX_m, cellCenterMapY_m - 1) !=
                        'W') ||
                   (nextDir_m == Down &&
                    map.getCellState(cellCenterMapX_m, cellCenterMapY_m + 1) !=
                        'W')) {
          if (cellCenterPixX_m == spriteCenterPixX_m) {
            dx_m = 0;
            dy_m = float(nextDir_m - 1) * moveDist;
            standardMove();
            currDir_m = nextDir_m;
          } else {
            moveDiagonal_m = true;
            currDir_m =
                (cellCenterPixX_m - spriteCenterPixX_m > 0) ? Right : Left;
            moveDiagonal(dTime);
          }
          hasTurned_m = true;
        }
      } else {
        switch (nextDir_m) {
          case Right:
            dx_m = moveDist;
            dy_m = 0;
            break;
          case Left:
            dx_m = -moveDist;
            dy_m = 0;
            break;
          case Up:
            dx_m = 0;
            dy_m = -moveDist;
            break;
          case Down:
            dx_m = 0;
            dy_m = moveDist;
            break;
          case Stop:
            dx_m = dy_m = 0;
            return;
        }
        sprite2H_m.move(dx_m, dy_m);
        if (!wallCollision(map)) {
          currDir_m = nextDir_m;
          spriteCenterPixX_m += dx_m;
          spriteCenterPixY_m += dy_m;
          updateVariables();
          coveredDistOneIter_m = moveDist;
          hasTurned_m = true;
        } else
          handleWallCollision(map, dTime, true);
      }
    }

    if (!hasTurned_m && currDir_m != Stop) {
      if (!simplifiedTurns_b) nextDir_m = currDir_m;
      switch (currDir_m) {
        case Right:
          dx_m = moveDist;
          dy_m = 0;
          break;
        case Left:
          dx_m = -moveDist;
          dy_m = 0;
          break;
        case Up:
          dx_m = 0;
          dy_m = -moveDist;
          break;
        case Down:
          dx_m = 0;
          dy_m = moveDist;
          break;
        case Stop:
          dx_m = dy_m = 0;
          return;
      }
      sprite2H_m.move(dx_m, dy_m);
      if (!wallCollision(map)) {
        spriteCenterPixX_m += dx_m;
        spriteCenterPixY_m += dy_m;
        updateVariables();
        coveredDistOneIter_m = moveDist;
      } else
        handleWallCollision(map, dTime, false);
    }
  }
}

void Pacman::updateDirection(sf::Event& event) {
  if (!moveDiagonal_m) {
    if (event.key.code == sf::Keyboard::Left ||
        event.key.code == sf::Keyboard::A) {
      nextDir_m = Left;
      hasTurned_m = true;
    } else if (event.key.code == sf::Keyboard::Right ||
               event.key.code == sf::Keyboard::D) {
      nextDir_m = Right;
      hasTurned_m = true;
    } else if (event.key.code == sf::Keyboard::Up ||
               event.key.code == sf::Keyboard::W) {
      nextDir_m = Up;
      hasTurned_m = true;
    } else if (event.key.code == sf::Keyboard::Down ||
               event.key.code == sf::Keyboard::S) {
      nextDir_m = Down;
      hasTurned_m = true;
    }
  }
}

void Pacman::handleWallCollision(Map& map, float dTime, bool isNextDir) {
  if (isNextDir)
    sprite2H_m.move(-dx_m, -dy_m);
  else {
    if (currDir_m == Stop)
      sprite2H_m.move(-dx_m, -dy_m);
    else
      sprite2H_m.setPosition(cellCenterPixX_m, cellCenterPixY_m);
    coveredDistOneIter_m = abs(spriteCenterPixX_m - cellCenterPixX_m) +
                           abs(spriteCenterPixY_m - cellCenterPixY_m);
    spriteCenterPixX_m = cellCenterPixX_m;
    spriteCenterPixY_m = cellCenterPixY_m;
    updateVariables();
    currDir_m = nextDir_m = Stop;
  }
}

void Pacman::animation(float dTime) {
  if (currDir_m == Stop) {
    accumTimeNextFrame_m = constTimeNextFrame_m;
    currFrame_m = 0;
    sprite2H_m.setTextureRect(
        {startTexRectL_m, startTexRectT_m, entitySize_i, entitySize_i});
    return;
  }
  accumTimeNextFrame_m += dTime;
  if (accumTimeNextFrame_m >= constTimeNextFrame_m) {
    while (accumTimeNextFrame_m >= constTimeNextFrame_m) {
      accumTimeNextFrame_m -= constTimeNextFrame_m;
      ++currFrame_m;
    }
    currFrame_m %= pacmanMaxFrameMove;
    sprite2H_m.setTextureRect({startTexRectL_m + currFrame_m * entitySize_i,
                               startTexRectT_m, entitySize_i, entitySize_i});
  }
  if (hasTurned_m) {
    hasTurned_m = false;
    if (moveDiagonal_m) {
      switch (currDir_m) {
        case Right:
          if (nextDir_m == Up)
            sprite2H_m.setRotation(315);
          else
            sprite2H_m.setRotation(45);
          break;
        case Left:
          if (nextDir_m == Up)
            sprite2H_m.setRotation(225);
          else
            sprite2H_m.setRotation(135);
          break;
        case Up:
          if (nextDir_m == Right)
            sprite2H_m.setRotation(315);
          else
            sprite2H_m.setRotation(225);
          break;
        case Down:
          if (nextDir_m == Right)
            sprite2H_m.setRotation(45);
          else
            sprite2H_m.setRotation(135);
          break;
        case Stop:
          return;
      }
    } else {
      switch (currDir_m) {
        case Right:
          sprite2H_m.setRotation(0);
          break;
        case Left:
          sprite2H_m.setRotation(180);
          break;
        case Up:
          sprite2H_m.setRotation(270);
          break;
        case Down:
          sprite2H_m.setRotation(90);
          break;
        case Stop:
          return;
      }
    }
  }
}

void Pacman::kill() { isKilled_m = true; }

void Pacman::killAnimation(ClassicGameMusic& CGMusic, float dTime) {
  if (!killAnimationEnded_m) {
    accumTimeNextFrame_m += dTime;
    if (accumTimeNextFrame_m >= pacmanTimeNextFrameKill) {
      if (!killFrames_m) {
        while (accumTimeNextFrame_m >= pacmanTimeNextFrameKill) {
          accumTimeNextFrame_m -= pacmanTimeNextFrameKill;
          if (currFrame_m == 5)
            currFrame_m = 4;
          else if (currFrame_m == 6)
            currFrame_m = 3;
          else if (currFrame_m == 7)
            currFrame_m = 2;
          else
            ++currFrame_m;
        }
        if (currFrame_m == 4) {
          currFrame_m = 0;
          killFrames_m = true;
          // first frame of kill animation
          sprite2H_m.setTextureRect({startTexRectL_m,
                                     startTexRectT_m + entitySize_i,
                                     entitySize_i, entitySize_i});
        } else
          // set ordinary frame
          sprite2H_m.setTextureRect(
              {startTexRectL_m + currFrame_m * entitySize_i, startTexRectT_m,
               entitySize_i, entitySize_i});
      } else {
        while (accumTimeNextFrame_m > pacmanTimeNextFrameKill) {
          accumTimeNextFrame_m -= pacmanTimeNextFrameKill;
          ++currFrame_m;
        }
        if (currFrame_m >= maxFrameKill_m) {
          currFrame_m = maxFrameKill_m - 1;
          // set black square
          sprite2H_m.setTextureRect(
              {startTexRectL_m + currFrame_m * entitySize_i, startTexRectT_m,
               entitySize_i, entitySize_i});
          killAnimationEnded_m = true;
          killFrames_m = false;
        } else {
          // set kill-animation frame
          sprite2H_m.setTextureRect(
              {startTexRectL_m + currFrame_m * entitySize_i,
               startTexRectT_m + entitySize_i, entitySize_i, entitySize_i});
          // handle death music
          if (currFrame_m == 11) {
            CGMusic.deathStartSound.stop();
            CGMusic.deathEndSound.play();
          }
        }
      }
    }
  }
}

void Pacman::setEndLevelAnimation() {
  sprite2H_m.setTextureRect(
      {startTexRectL_m, startTexRectT_m, entitySize_i, entitySize_i});
}

int Pacman::getExtraLives() const { return extraLives_m; }

bool Pacman::isKilled() const { return isKilled_m; }

bool Pacman::killAnimationEnded() const { return killAnimationEnded_m; }

float Pacman::getCoveredDistOneIter() const { return coveredDistOneIter_m; }

void Pacman::addExtraLife() { ++extraLives_m; }

void Pacman::setNormalSpeed(bool frightMode) {
  if (frightMode)
    currSpeed_m = frightSpeed_m;
  else
    currSpeed_m = normalSpeed_m;
}

void Pacman::setDotsSpeed(bool frightMode) {
  if (frightMode)
    currSpeed_m = frightDotsSpeed_m;
  else
    currSpeed_m = normalDotsSpeed_m;
}

void Pacman::previewSetStart(float posX, float posY) {
  sprite2H_m.setTextureRect(
      {startTexRectL_m, startTexRectT_m, entitySize_i, entitySize_i});
  sprite2H_m.setRotation(180);
  sprite2H_m.setPosition(posX, posY);
  currDir_m = Dir::Left;
  currSpeed_m = 0.8 * maxSpeed;
}

void Pacman::previewUpdate(float dTime) {
  // move
  float moveDist = currSpeed_m * dTime;
  if (currDir_m == Dir::Left)
    sprite2H_m.move(-moveDist, 0);
  else
    sprite2H_m.move(moveDist, 0);

  // animation
  accumTimeNextFrame_m += dTime;
  if (accumTimeNextFrame_m >= constTimeNextFrame_m) {
    while (accumTimeNextFrame_m >= constTimeNextFrame_m) {
      accumTimeNextFrame_m -= constTimeNextFrame_m;
      ++currFrame_m;
    }
    currFrame_m %= pacmanMaxFrameMove;
    sprite2H_m.setTextureRect({startTexRectL_m + currFrame_m * entitySize_i,
                               startTexRectT_m, entitySize_i, entitySize_i});
  }
}

void Pacman::previewChangeBehaviour() {
  sprite2H_m.setTextureRect(
      {startTexRectL_m, startTexRectT_m, entitySize_i, entitySize_i});
  sprite2H_m.setRotation(0);
  currDir_m = Dir::Right;
  currSpeed_m = 0.9 * maxSpeed;
}

void Pacman::reloadAfterDeath() {
  --extraLives_m;

  Entity::reloadAfterDeath();
  killFrames_m = false;
  isKilled_m = false;
  killAnimationEnded_m = false;
  moveDiagonal_m = false;
  coveredDistOneIter_m = 0;
  currSpeed_m = normalSpeed_m;
}

void Pacman::reloadNextLevel(
    int level) {  // todo: reloadNextLevel very similar to reloadNewGame
  updateSpeed(level);
  calculateTimeConstants();

  Entity::reloadAfterDeath();
  killFrames_m = false;
  isKilled_m = false;
  killAnimationEnded_m = false;
  moveDiagonal_m = false;
  coveredDistOneIter_m = 0;
  currSpeed_m = normalSpeed_m;
}

void Pacman::reloadNewGame() {
  reloadNextLevel(1);
  extraLives_m = pacmanStartNumOfExtraLives;
}