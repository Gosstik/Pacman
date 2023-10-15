#include "Ghost.h"

const std::vector<std::pair<int, int>> Ghost::cellsForbadeUpDir{
    {14, 11}, {17, 11}, {14, 23}, {17, 23}};

void Ghost::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite2H_m);
  if (showGhostsTargets_b && !ust_m.bigGumEffect &&
      (st_m.alive || st_m.moveToCage) && !ust_m.previewMode)
    target.draw(target_m.spr);
}

Ghost::Ghost(const sf::Texture& texture, int ghostStartPosMapX,
             int ghostStartPosMapY, float ghostStartPosPixX,
             float ghostStartPosPixY, int ghostStartTexRectL,
             int ghostStartTexRectT, int level)
    : Entity(texture, entityHbWidth, ghostStartPosMapX, ghostStartPosMapY,
             ghostStartPosPixX, ghostStartPosPixY, ghostStartTexRectL,
             ghostStartTexRectT),
      gen_m(seed_m()),
      target_m(texture) {
  // setting state booleans
  if (ghostStartPosPixY == cageCenterPixY &&
      (ghostStartPosPixX == cageCenterPixX ||
       ghostStartPosPixX == cageLeftCellPixX ||
       ghostStartPosPixX == cageRightCellPixX))
    ust_m.startInCage = st_m.simpleMoveInCage = true;
  else
    ust_m.startInCage = st_m.simpleMoveInCage = false;
  st_m.alive = !ust_m.startInCage;

  // setting directions
  vecDir_m.reserve(4);
  vecDist_m.resize(3);
  if (st_m.alive)
    currDir_m = nextDir_m = Left;
  else {
    if (spriteCenterPixX_m == cageCenterPixX)
      currDir_m = nextDir_m = Down;
    else
      currDir_m = nextDir_m = Up;
  }
}

void Ghost::calculateTimeConstants() {
  // ghost uses ghostMaxFrameMove frames moving each cell
  tm_m.constSimpleNextFrame =
      cellSize_f / normalSpeed_m / float(ghostMaxFrameMove) * 2.0f;
  tm_m.constFrightNextFrame =
      cellSize_f / frightSpeed_m / float(ghostMaxFrameMove) * 2.0f;
}

// Ghost::Ghost(const Ghost& other) : Entity(other),
// ghostMaxFrameMove(other.ghostMaxFrameMove), startInCage(other.startInCage),
// gen_m(other.gen_m) {
//     tm_m = other.tm_m;
//     st_m = other.st_m;
//     ust_m = other.ust_m;
//
//     hasTurned_m = other.hasTurned_m;
//     frightColor_m = other.frightColor_m;
//     queue_m = other.queue_m;
// }

// Ghost& Ghost::operator=(const Ghost& other) {
//     Ghost copy = other;
//     Entity::operator=(other);
//     mySwap(copy);
//     return *this;
// }

// Ghost::Ghost(Ghost&& other) noexcept;
// Ghost& Ghost::operator=(Ghost&& other) noexcept;

// void Ghost::mySwap(Ghost& other) {
//     tm_m = other.tm_m;
//     st_m = other.st_m;
//     ust_m = other.ust_m;
//
//     hasTurned_m = other.hasTurned_m;
//     frightColor_m = other.frightColor_m;
//     std::swap(queue_m, other.queue_m);
// }

void Ghost::updateDirection(Map& map, Pacman& pacman, Blinky& blinky,
                            const GhostsMode& mode) {
  updateVecDir(map);
  switch (mode) {
    case scatter:
      scatterChooseTarget(pacman);
      choosePreferableDirection();
      break;
    case chase:
      chaseChooseTarget(pacman, blinky);
      choosePreferableDirection();
      break;
    case frightened:
      currDir_m = vecDir_m[gen_m() % vecDir_m.size()];
      break;
    case dead:
      choosePreferableDirection();
      break;
  }
}

void Ghost::setScatterMode(Map& map, Pacman& pacman) {
  if (st_m.alive) {
    scatterChooseTarget(pacman);
    simpleReverseDir();
  }
}

void Ghost::setChaseMode(Map& map, Pacman& pacman, Blinky& blinky) {
  if (st_m.alive) {
    chaseChooseTarget(pacman, blinky);
    simpleReverseDir();
  }
}

void Ghost::choosePreferableDirection() {
  if (vecDir_m.size() == 1)
    currDir_m = vecDir_m[0];
  else {
    int minDist = distToTarget(vecDir_m[0]);
    currDir_m = vecDir_m[0];
    int tmpDist;
    for (size_t i = 1; i < vecDir_m.size(); ++i) {
      tmpDist = distToTarget(vecDir_m[i]);
      if (tmpDist < minDist) {
        minDist = tmpDist;
        currDir_m = vecDir_m[i];
      } else if (tmpDist == minDist && vecDir_m[i] < currDir_m) {
        currDir_m = vecDir_m[i];
      }
    }
  }
}

int Ghost::distToTarget(const Dir& dir) {
  int X = cellCenterMapX_m - target_m.mapX;
  int Y = cellCenterMapY_m - target_m.mapY;
  switch (dir) {
    case Right:
      return (X + 1) * (X + 1) + Y * Y;
    case Left:
      return (X - 1) * (X - 1) + Y * Y;
    case Up:
      return X * X + (Y - 1) * (Y - 1);
    case Down:
      return X * X + (Y + 1) * (Y + 1);
    case Stop:
      return X * X + Y * Y;
  }
  std::cerr << "Ghost::distToTarget : dir is incorrect\n";
  std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
  exit(8);
}

int Ghost::distToCell(int x, int y) {
  int X = cellCenterMapX_m - x;
  int Y = cellCenterMapY_m - y;
  int res = X * X + Y * Y;
  return res;
}

void Ghost::updateVecDir(Map& map) {
  vecDir_m.clear();
  if (map[cellCenterMapY_m][cellCenterMapX_m + 1] != 'W')
    vecDir_m.emplace_back(Right);
  if (map[cellCenterMapY_m][cellCenterMapX_m - 1] != 'W')
    vecDir_m.emplace_back(Left);
  if (map[cellCenterMapY_m - 1][cellCenterMapX_m] != 'W')
    vecDir_m.emplace_back(Up);
  if (map[cellCenterMapY_m + 1][cellCenterMapX_m] != 'W')
    vecDir_m.emplace_back(Down);
  // to forbid ghost to reverse direction
  if (vecDir_m.empty()) {
    std::cerr << "Ghost::updateVecDir() : 111 vecDir_m.empty()\n";
    std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
    exit(111);
  }
  vecDir_m.erase(
      std::find(vecDir_m.begin(), vecDir_m.end(), (currDir_m + 2) % 4));
  if (vecDir_m.empty()) {
    std::cerr << "Ghost::updateVecDir() : 222 vecDir_m.empty()\n";
    std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
    exit(222);
  }
  if (!ust_m.bigGumEffect)
    for (auto& cell : cellsForbadeUpDir)
      if (cellCenterMapX_m == cell.first && cellCenterMapY_m == cell.second) {
        auto tmp = std::find(vecDir_m.begin(), vecDir_m.end(), Dir::Up);
        if (tmp != vecDir_m.end()) vecDir_m.erase(tmp);
        break;
      }
  if (vecDir_m.empty()) {
    std::cerr << "Ghost::updateVecDir() : 333 vecDir_m.empty()\n";
    std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
    exit(333);
  }
}

void Ghost::getDestination(std::vector<Ghost*>& cagePopulation,
                           const Ghost* releasingGhost) {
  size_t i;
  if (releasingGhost) {
    i = 1;
    while (i < cagePopulation.size()) {
      if (cagePopulation[i] == nullptr) break;
      ++i;
    }
    if (i == 3) i = 0;
  } else {
    i = 0;
    while (i < cagePopulation.size()) {
      if (cagePopulation[i] == nullptr) break;
      ++i;
    }
  }

  while (!queue_m.empty()) queue_m.pop();
  switch (i) {
    case 0:
      cagePopulation[0] = this;
      nextDir_m = Down;
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Down);
      break;
    case 1:
      cagePopulation[1] = this;
      nextDir_m = Up;
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Left);
      queue_m.push(Dir::Left);
      break;
    case 2:
      cagePopulation[2] = this;
      nextDir_m = Up;
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Down);
      queue_m.push(Dir::Right);
      queue_m.push(Dir::Right);
      break;
    case 3:
      break;
    default:
      std::cerr << "Ghost::getDestination : i > 3\n";
      std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
      exit(9);
  }
}

void Ghost::update(MyWindow& window, Map& map, Pacman& pacman, Blinky& blinky,
                   GhostsStates& gsSt, GhostsTimes& gsTm,
                   ClassicGameMusic& CGMusic, GhostsMode& mode,
                   std::vector<Ghost*>& cagePopulation,
                   const Ghost* releasingGhost, float dTime) {
  handleSpeed();
  if (ust_m.bigGumEffect && !gsSt.bigGumEffect) removeBigGumEffect();
  if (st_m.alive)
    move(map, pacman, blinky, mode, dTime);
  else if (st_m.simpleMoveInCage)
    simpleMoveInCage(dTime);
  else if (st_m.moveToCage)
    moveToCage(map, pacman, blinky, cagePopulation, releasingGhost, dTime);
  else if (st_m.releasing)
    moveReleaseFromCage(dTime);
  else if (st_m.moveToCageCentralCell)
    moveToCageCentralCell(dTime);
  else {
    std::cerr << "Ghost::recount : no boolean is true\n";
    std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
    exit(50);
  }

  if (st_m.moveToCage)
    animationMoveToCage();
  else if (ust_m.bigGumEffect)
    animationBigGumEffect(gsTm, dTime);
  else
    animation(dTime);
}

void Ghost::handleSpeed() {
  if (st_m.alive) {
    // it means that ghost is too close to the portal
    if (spriteCenterPixY_m == leftPortalCenterPixY &&
        (spriteCenterPixX_m <= leftPortalCenterPixX + 6 * cellSize_f ||
         spriteCenterPixX_m >= rightPortalCenterPixX - 6 * cellSize_f)) {
      currSpeed_m = tunnelSpeed_m;
    } else if (ust_m.bigGumEffect)
      currSpeed_m = frightSpeed_m;
    else
      currSpeed_m = normalSpeed_m;
  } else if (st_m.moveToCage)
    currSpeed_m = moveToCageSpeed_m;
  else
    currSpeed_m = cageSpeed_m;
}

void Ghost::move(Map& map, Pacman& pacman, Blinky& blinky,
                 const GhostsMode& mode, float dTime) {
  float moveDist = currSpeed_m * dTime;
  handlePortal(moveDist);
  // if ghost is too close to the cell center it moves
  // to the center instead of moving on dTime * currSpeed_m
  // in order to find turn on new direction correctly
  if ((currDir_m == Left && spriteCenterPixX_m >= cellCenterPixX_m &&
       spriteCenterPixX_m - cellCenterPixX_m <= moveDist) ||
      (currDir_m == Right && cellCenterPixX_m >= spriteCenterPixX_m &&
       cellCenterPixX_m - spriteCenterPixX_m <= moveDist) ||
      (currDir_m == Up && spriteCenterPixY_m >= cellCenterPixY_m &&
       spriteCenterPixY_m - cellCenterPixY_m <= moveDist) ||
      (currDir_m == Down && cellCenterPixY_m >= spriteCenterPixY_m &&
       cellCenterPixY_m - spriteCenterPixY_m <= moveDist)) {
    sprite2H_m.setPosition(cellCenterPixX_m, cellCenterPixY_m);
    spriteCenterPixX_m = cellCenterPixX_m;
    spriteCenterPixY_m = cellCenterPixY_m;
    moveDist -= abs(spriteCenterPixX_m - cellCenterPixX_m) +
                abs(spriteCenterPixY_m - cellCenterPixY_m);
    // if moveDist == 0 and on this iteration we try to reverse direction
    // then ghost will move on wall. Therefore, further we check wallCollision
    updateDirection(map, pacman, blinky, mode);
    hasTurned_m = true;
  }
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
      std::cerr << "Ghost::move() : 1 vecDir_m == Stop\n";
      std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
      exit(201);
  }
  sprite2H_m.move(dx_m, dy_m);
  // this may be in case described above
  if (wallCollision(map)) {
    sprite2H_m.move(-dx_m, -dy_m);
    updateDirection(map, pacman, blinky, mode);
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
        std::cerr << "Ghost::move : 2 vecDir_m == Stop\n";
        std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
        exit(202);
    }
    sprite2H_m.move(dx_m, dy_m);
  }
  spriteCenterPixX_m += dx_m;
  spriteCenterPixY_m += dy_m;
  updateVariables();
}

void Ghost::animation(float dTime) {
  accumTimeNextFrame_m += dTime;
  if (hasTurned_m) {
    hasTurned_m = false;
    sprite2H_m.setTextureRect(
        {(ghostMaxFrameMove * currDir_m + currFrame_m) * entitySize_i,
         startTexRectT_m, entitySize_i, entitySize_i});
  }
  if (accumTimeNextFrame_m >= tm_m.constSimpleNextFrame) {
    while (accumTimeNextFrame_m >= tm_m.constSimpleNextFrame) {
      accumTimeNextFrame_m -= tm_m.constSimpleNextFrame;
      ++currFrame_m;
    }
    currFrame_m %= ghostMaxFrameMove;
    sprite2H_m.setTextureRect(
        {(ghostMaxFrameMove * currDir_m + currFrame_m) * entitySize_i,
         startTexRectT_m, entitySize_i, entitySize_i});
  }
}

void Ghost::setBigGumEffect(Map& map) {
  if (!st_m.moveToCage) {
    hasTurned_m = true;
    ust_m.bigGumEffect = true;
    ust_m.startChangeFrightColor = true;
    frightColor_m = Blue;
    tm_m.accumChangeFrightColor = 0;
    if (st_m.simpleMoveInCage || st_m.alive) simpleReverseDir();
  }
}

void Ghost::animationBigGumEffect(GhostsTimes& gsTm, float dTime) {
  accumTimeNextFrame_m += dTime;
  if (hasTurned_m) {
    hasTurned_m = false;
    int rectLeft =
        frightGhostRectLeft +
        (frightColor_m * ghostMaxFrameMove + currFrame_m) * entitySize_i;
    sprite2H_m.setTextureRect(
        {rectLeft, frightGhostRectTop, entitySize_i, entitySize_i});
  }
  if (accumTimeNextFrame_m >= tm_m.constFrightNextFrame) {
    while (accumTimeNextFrame_m >= tm_m.constFrightNextFrame) {
      accumTimeNextFrame_m -= tm_m.constFrightNextFrame;
      ++currFrame_m;
    }
    currFrame_m %= ghostMaxFrameMove;
    int rectLeft =
        frightGhostRectLeft +
        (frightColor_m * ghostMaxFrameMove + currFrame_m) * entitySize_i;
    sprite2H_m.setTextureRect(
        {rectLeft, frightGhostRectTop, entitySize_i, entitySize_i});
  }
  if (gsTm.frightTime - gsTm.accumTimeBigGumEffect <=
      timeStartChangeFrightColor) {
    if (ust_m.startChangeFrightColor) {
      changeFrightColor();
      ust_m.startChangeFrightColor = false;
      tm_m.accumChangeFrightColor =
          timeStartChangeFrightColor -
          (gsTm.frightTime - gsTm.accumTimeBigGumEffect);
      return;
    }
    tm_m.accumChangeFrightColor += dTime;
    if (tm_m.accumChangeFrightColor >= timeChangeFrightColor) {
      while (tm_m.accumChangeFrightColor >= timeChangeFrightColor) {
        tm_m.accumChangeFrightColor -= timeChangeFrightColor;
        changeFrightColor();
      }
      int rectLeft =
          frightGhostRectLeft +
          (frightColor_m * ghostMaxFrameMove + currFrame_m) * entitySize_i;
      sprite2H_m.setTextureRect(
          {rectLeft, frightGhostRectTop, entitySize_i, entitySize_i});
    }
  }
}

void Ghost::kill(Map& map, std::vector<Ghost*>& cagePopulation) {
  st_m.alive = false;
  if (ust_m.bigGumEffect) removeBigGumEffect();
  ust_m.inPlace = false;
  st_m.moveToCage = true;
  ust_m.startMoveToCage = true;
  target_m.setPosition(rightAboveCageCenterMapX, aboveCageCenterMapY);
}

void Ghost::moveToCage(Map& map, Pacman& pacman, Blinky& blinky,
                       std::vector<Ghost*>& cagePopulation,
                       const Ghost* releasingGhost, float dTime) {
  if (!ust_m.reachedAboveCageCell) {
    move(map, pacman, blinky, GhostsMode::dead, dTime);
    if (cellCenterMapY_m == aboveCageCenterMapY) {
      if (cellCenterMapX_m == leftAboveCageCenterMapX) {
        currDir_m = Right;
        hasTurned_m = true;
        getDestination(cagePopulation, releasingGhost);
        ust_m.reachedAboveCageCell = true;
        target_m.setDefault();
      } else if (cellCenterMapX_m == rightAboveCageCenterMapX) {
        currDir_m = Left;
        hasTurned_m = true;
        getDestination(cagePopulation, releasingGhost);
        ust_m.reachedAboveCageCell = true;
        target_m.setDefault();
      }
    }
  } else if (!ust_m.reachedAboveCagePos) {
    if ((currDir_m == Left &&
         spriteCenterPixX_m - aboveCageCenterPixX < dTime * currSpeed_m) ||
        (currDir_m == Right &&
         aboveCageCenterPixX - spriteCenterPixX_m < dTime * currSpeed_m)) {
      sprite2H_m.setPosition(aboveCageCenterPixX, aboveCageCenterPixY);
      if (queue_m.empty()) {
        currDir_m = Left;
        st_m.moveToCage = false;
        st_m.alive = true;
        ust_m.reachedAboveCageCell = false;
      } else {
        ust_m.reachedAboveCagePos = true;
        currDir_m = queue_m.front();
        queue_m.pop();
        hasTurned_m = true;
      }
      return;
    } else {
      if (currDir_m == Right)
        dx_m = dTime * currSpeed_m;
      else
        dx_m = -dTime * currSpeed_m;
      sprite2H_m.move(dx_m, 0);
      spriteCenterPixX_m += dx_m;
    }
  } else {
    if (cellSize_f - accumDistMoveToCage_m <= currSpeed_m * dTime) {
      switch (currDir_m) {
        case Right:
          dx_m = cellSize_f - accumDistMoveToCage_m;
          dy_m = 0;
          break;
        case Left:
          dx_m = accumDistMoveToCage_m - cellSize_f;
          dy_m = 0;
          break;
        case Up:
          dx_m = 0;
          dy_m = accumDistMoveToCage_m - cellSize_f;
          break;
        case Down:
          dx_m = 0;
          dy_m = cellSize_f - accumDistMoveToCage_m;
          break;
        case Stop:
          std::cerr << "Ghost::moveToCage() : vecDir_m == Stop\n";
          std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
          exit(100);
      }
      sprite2H_m.move(dx_m, dy_m);
      accumDistMoveToCage_m = 0;
      if (queue_m.empty()) {
        currDir_m = nextDir_m;
        nextDir_m = Stop;
        st_m.moveToCage = false;
        ust_m.inPlace = true;
        st_m.simpleMoveInCage = true;
        hasTurned_m = true;
        ust_m.reachedAboveCageCell = false;
        ust_m.reachedAboveCagePos = false;
        spriteCenterPixX_m = sprite2H_m.getPosition().x;
        spriteCenterPixY_m = sprite2H_m.getPosition().y;
        updateVariables();  // todo: can be removed
        return;
      } else {
        currDir_m = queue_m.front();
        queue_m.pop();
        hasTurned_m = true;
      }
    } else {
      accumDistMoveToCage_m += currSpeed_m * dTime;
      switch (currDir_m) {
        case Right:
          dx_m = currSpeed_m * dTime;
          dy_m = 0;
          break;
        case Left:
          dx_m = -currSpeed_m * dTime;
          dy_m = 0;
          break;
        case Up:
          dx_m = 0;
          dy_m = -currSpeed_m * dTime;
          break;
        case Down:
          dx_m = 0;
          dy_m = currSpeed_m * dTime;
          break;
        case Stop:
          std::cerr << "Ghost::moveToCage() : vecDir_m == Stop\n";
          std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
          exit(100);
      }
      sprite2H_m.move(dx_m, dy_m);
    }
  }
}

void Ghost::animationMoveToCage() {
  if (ust_m.startMoveToCage) {
    ust_m.startMoveToCage = false;
    accumTimeNextFrame_m = 0;
    sprite2H_m.setTextureRect({ghostEyesRectLeft + currDir_m * entitySize_i,
                               ghostEyesRectTop, entitySize_i, entitySize_i});
  } else if (hasTurned_m) {
    hasTurned_m = false;
    sprite2H_m.setTextureRect({ghostEyesRectLeft + currDir_m * entitySize_i,
                               ghostEyesRectTop, entitySize_i, entitySize_i});
  }
}

void Ghost::simpleMoveInCage(float dTime) {
  // if ghost is too close to the cell center it
  // moves to the extreme position instead of moving on
  // dTime * currSpeed_m in order to find turn on new direction correctly
  if (currDir_m == Up && spriteCenterPixY_m > cageMinCenterPixY &&
      spriteCenterPixY_m - cageMinCenterPixY < dTime * currSpeed_m) {
    sprite2H_m.setPosition(spriteCenterPixX_m, cageMinCenterPixY);
    spriteCenterPixY_m = cageMinCenterPixY;
    return;
  } else if (currDir_m == Down && cageMaxCenterPixY > spriteCenterPixY_m &&
             cageMaxCenterPixY - spriteCenterPixY_m < dTime * currSpeed_m) {
    sprite2H_m.setPosition(spriteCenterPixX_m, cageMaxCenterPixY);
    spriteCenterPixY_m = cageMaxCenterPixY;
    return;
  }
  // if ghost is in the extreme position next direction should be chosen
  if (spriteCenterPixY_m == cageMinCenterPixY) {
    currDir_m = Down;
    dy_m = dTime * currSpeed_m;
    hasTurned_m = true;
  } else if (spriteCenterPixY_m == cageMaxCenterPixY) {
    currDir_m = Up;
    dy_m = -dTime * currSpeed_m;
    hasTurned_m = true;
  } else
    dy_m = (currDir_m == Down) ? dTime * currSpeed_m : -dTime * currSpeed_m;
  sprite2H_m.move(0, dy_m);
  spriteCenterPixY_m += dy_m;
}

void Ghost::setToCageCenterCell() {
  ust_m.inPlace = false;
  st_m.simpleMoveInCage = false;
  st_m.moveToCageCentralCell = true;
  hasTurned_m = true;
  if (spriteCenterPixX_m - cageCenterPixX > 0)
    currDir_m = Left;
  else
    currDir_m = Right;
}

void Ghost::moveToCageCentralCell(float dTime) {
  // if ghost is too close to the cell center it
  // moves to the center instead of moving on dTime * currSpeed_m
  // in order to reach cell center in any case
  if ((currDir_m == Left &&
       spriteCenterPixX_m - cageCenterPixX < dTime * currSpeed_m) ||
      (currDir_m == Right &&
       cageCenterPixX - spriteCenterPixX_m < dTime * currSpeed_m)) {
    sprite2H_m.setPosition(cageCenterPixX, spriteCenterPixY_m);
    spriteCenterPixX_m = cageCenterPixX;
    updateVariables();
    currDir_m = Up;
    hasTurned_m = true;
    st_m.moveToCageCentralCell = false;
    st_m.releasing = true;
  } else {
    switch (currDir_m) {
      case Right:
        dx_m = dTime * currSpeed_m;
        break;
      case Left:
        dx_m = -dTime * currSpeed_m;
        break;
      default:
        std::cerr << "Ghost::moveToCageCentralCell : currDir_m is incorrect\n";
        std::cerr << "Ghost name: " << typeid(*this).name() << "\n";
        exit(8);
    }
    sprite2H_m.move(dx_m, 0);
    spriteCenterPixX_m += dx_m;
  }
}

void Ghost::releaseFromCage() {
  if (spriteCenterPixX_m == cageCenterPixX) {
    st_m.simpleMoveInCage = false;
    ust_m.inPlace = false;
    st_m.releasing = true;
  } else
    setToCageCenterCell();
}

void Ghost::moveReleaseFromCage(float dTime) {
  // if ghost is higher than the extreme position of cage movement
  if (spriteCenterPixY_m <= cageMinCenterPixY) {
    // if ghost is too close to the cell center it
    // moves to the center instead of moving on dTime * currSpeed_m
    // in order to reach cell center in any case
    if (spriteCenterPixY_m >= aboveCageCenterPixY &&
        spriteCenterPixY_m - aboveCageCenterPixY <= dTime * currSpeed_m) {
      sprite2H_m.setPosition(spriteCenterPixX_m, aboveCageCenterPixY);
      spriteCenterPixY_m = aboveCageCenterPixY;
      updateVariables();
      st_m.releasing = false;
      ust_m.inPlace = true;
      st_m.alive = true;
      hasTurned_m = true;
      currDir_m = Left;
    } else {
      dy_m = -dTime * currSpeed_m;
      sprite2H_m.move(0, dy_m);
      spriteCenterPixY_m += dy_m;
      updateVariables();
    }
    return;
  }
  // if ghost is too close to the cell center it
  // moves to the center (or extreme position) instead of
  // moving on dTime * currSpeed_m in order to find turn on
  // new direction correctly
  else if (currDir_m == Up && spriteCenterPixY_m > cageMinCenterPixY &&
           spriteCenterPixY_m - cageMinCenterPixY < dTime * currSpeed_m) {
    sprite2H_m.setPosition(spriteCenterPixX_m, cageMinCenterPixY);
    spriteCenterPixY_m = cageMinCenterPixY;
    updateVariables();
    return;
  } else if (currDir_m == Down && cageMaxCenterPixY > spriteCenterPixY_m &&
             cageMaxCenterPixY - spriteCenterPixY_m < dTime * currSpeed_m) {
    sprite2H_m.setPosition(spriteCenterPixX_m, cageMaxCenterPixY);
    spriteCenterPixY_m = cageMaxCenterPixY;
    updateVariables();
    return;
  }
  // if ghost is in the extreme position of cage movement that is
  // farther to the cage exit then ghost start move Up
  else if (spriteCenterPixY_m == cageMaxCenterPixY) {
    currDir_m = Up;
    dy_m = -dTime * currSpeed_m;
    hasTurned_m = true;
  } else
    dy_m = (currDir_m == Down) ? dTime * currSpeed_m : -dTime * currSpeed_m;
  sprite2H_m.move(0, dy_m);
  spriteCenterPixY_m += dy_m;
  updateVariables();
}

void Ghost::previewSetStart(float posX, float posY) {
  ust_m.previewMode = true;
  sprite2H_m.setTextureRect(
      {previewStartTexRectL_m, startTexRectT_m, entitySize_i, entitySize_i});
  sprite2H_m.setPosition(posX, posY);
  spriteCenterPixX_m = posX;
  spriteCenterPixY_m = posY;
  currDir_m = Dir::Left;
  currSpeed_m = 0.85 * maxSpeed;
  st_m.alive = true;
  ust_m.bigGumEffect = false;
}

void Ghost::previewUpdate(float dTime) {
  if (st_m.alive) {
    // move
    float moveDist = currSpeed_m * dTime;
    if (currDir_m == Dir::Left)
      sprite2H_m.move(-moveDist, 0);
    else
      sprite2H_m.move(moveDist, 0);
    spriteCenterPixX_m = sprite2H_m.getPosition().x;
    spriteCenterPixY_m = sprite2H_m.getPosition().y;

    // animation
    if (ust_m.bigGumEffect) {
      accumTimeNextFrame_m += dTime;
      if (accumTimeNextFrame_m >= tm_m.constFrightNextFrame) {
        while (accumTimeNextFrame_m >= tm_m.constFrightNextFrame) {
          accumTimeNextFrame_m -= tm_m.constFrightNextFrame;
          ++currFrame_m;
        }
        currFrame_m %= ghostMaxFrameMove;
        int rectLeft = frightGhostRectLeft + currFrame_m * entitySize_i;
        sprite2H_m.setTextureRect(
            {rectLeft, frightGhostRectTop, entitySize_i, entitySize_i});
      }
    } else
      animation(dTime);
  }
}

void Ghost::previewChangeBehaviour() {
  currDir_m = Dir::Right;
  currSpeed_m = frightSpeed_m;
  ust_m.bigGumEffect = true;

  int rectLeft = frightGhostRectLeft + currFrame_m * entitySize_i;
  sprite2H_m.setTextureRect(
      {rectLeft, frightGhostRectTop, entitySize_i, entitySize_i});
}

void Ghost::previewKill() {
  st_m.alive = false;
  sprite2H_m.setPosition(viewWidthPix_f + entitySize_f, 0);
  spriteCenterPixX_m = viewWidthPix_f + entitySize_f;
  spriteCenterPixY_m = 0;
}

void Ghost::previewUpdateSpeed(float normalSpeed, float frightSpeed) {
  normalSpeed_m = normalSpeed;
  frightSpeed_m = frightSpeed;
}

bool Ghost::isAlive() const { return st_m.alive; }

bool Ghost::isInPlace() const { return ust_m.inPlace; }

void Ghost::changeFrightColor() {
  frightColor_m = (frightColor_m == Blue) ? White : Blue;
}

void Ghost::removeBigGumEffect() {
  ust_m.bigGumEffect = false;
  ust_m.startChangeFrightColor = false;
  tm_m.accumChangeFrightColor = 0;
  frightColor_m = Blue;
  hasTurned_m = true;
}

bool Ghost::canBeEaten() const { return ust_m.bigGumEffect; }

bool Ghost::isMovingToCage() const { return st_m.moveToCage; }

bool Ghost::underBigGumEffect() const { return ust_m.bigGumEffect; }

void Ghost::updateSpeed(float normalSpeed, float tunnelSpeed,
                        float frightSpeed) {
  normalSpeed_m = normalSpeed;
  moveToCageSpeed_m = normalSpeed * 1.5f;
  cageSpeed_m = tunnelSpeed_m = tunnelSpeed;
  frightSpeed_m = frightSpeed;
  if (st_m.alive)
    currSpeed_m = normalSpeed_m;
  else
    currSpeed_m = cageSpeed_m;
}

void Ghost::simpleReverseDir() {
  currDir_m = static_cast<Dir>((currDir_m + 2) % 4);
  hasTurned_m = true;
}

sf::IntRect Ghost::getTextureRect(const Dir& dir) {
  return {ghostMaxFrameMove * dir * entitySize_i, startTexRectT_m, entitySize_i,
          entitySize_i};
}

void Ghost::reloadAfterPacmanDeath() {
  // Entity
  Entity::reloadAfterDeath();

  // Ghost
  tm_m.setDefault();
  st_m.setDefault();
  ust_m.setDefault();

  st_m.alive = !ust_m.startInCage;
  st_m.simpleMoveInCage = ust_m.startInCage;
  if (st_m.alive)
    currDir_m = nextDir_m = Left;
  else {
    if (spriteCenterPixX_m == cageCenterPixX)
      currDir_m = nextDir_m = Down;
    else
      currDir_m = nextDir_m = Up;
  }

  // additional
  frightColor_m = Blue;
  vecDir_m.clear();
  while (!queue_m.empty()) queue_m.pop();

  vecDist_m.clear();
  target_m.setDefault();
  accumDistMoveToCage_m = 0;
  justBeenKilled_m = false;
}

void Ghost::reloadNextLevel(
    int level, float normalSpeed, float tunnelSpeed,
    float frightSpeed) {  // todo: reloadNewGame == reloadNextLevel
  // !!! pinky has custom reloadAfterPacmanDeath()
  reloadAfterPacmanDeath();
  // depend on ghost state
  updateSpeed(normalSpeed, tunnelSpeed, frightSpeed);
  // depend on speed
  calculateTimeConstants();
}

void Ghost::reloadNewGame(float normalSpeed, float tunnelSpeed,
                          float frightSpeed) {
  // !!! pinky has custom reloadAfterPacmanDeath()
  reloadAfterPacmanDeath();
  // depend on ghost state
  updateSpeed(normalSpeed, tunnelSpeed, frightSpeed);
  // depend on speed
  calculateTimeConstants();
}