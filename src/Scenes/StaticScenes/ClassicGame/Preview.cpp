#include "Preview.h"

void Preview::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (!startTurnOff_m) {
    if (showPressSpace_m) target.draw(pressSpace_m);
    switch (state_m) {
      case move:
        if (someGhostWasKilled_m) target.draw(killScoreSpr_m);
        drawGhostsWithBigGumEffect(target);
        if (!someGhostWasKilled_m) target.draw(pacman_m);
        drawGhostsWithoutBigGumEffect(target);
      case nickname:
        target.draw(copyright_m);
        target.draw(author_m);
        if (!bigGumWasEaten_m && showBigGums_m) target.draw(bigGum_m);
      case gums:
        target.draw(gumPic_m);
        target.draw(ten_m);
        target.draw(pts1_m);
        if (showBigGums_m) target.draw(bigGumPic_m);
        target.draw(fifty_m);
        target.draw(pts2_m);
      case clydeText:
        target.draw(clydeText_m);
      case pokey:
        target.draw(pokey_m);
      case clydePic:
        target.draw(clydePic_m);
      case inkyText:
        target.draw(inkyText_m);
      case bashful:
        target.draw(bashful_m);
      case inkyPic:
        target.draw(inkyPic_m);
      case pinkyText:
        target.draw(pinkyText_m);
      case speedy:
        target.draw(speedy_m);
      case pinkyPic:
        target.draw(pinkyPic_m);
      case blinkyText:
        target.draw(blinkyText_m);
      case shadow:
        target.draw(shadow_m);
      case blinkyPic:
        target.draw(blinkyPic_m);
      case start:
        target.draw(CharAndNick_m);
    }
  }
}

void Preview::drawGhostsWithoutBigGumEffect(sf::RenderTarget& target) const {
  if (!blinky_m.underBigGumEffect() && !blinky_m.justBeenKilled_m)
    target.draw(blinky_m);
  if (!pinky_m.underBigGumEffect() && !pinky_m.justBeenKilled_m)
    target.draw(pinky_m);
  if (!inky_m.underBigGumEffect() && !inky_m.justBeenKilled_m)
    target.draw(inky_m);
  if (!clyde_m.underBigGumEffect() && !clyde_m.justBeenKilled_m)
    target.draw(clyde_m);
}

void Preview::drawGhostsWithBigGumEffect(sf::RenderTarget& target) const {
  if (blinky_m.underBigGumEffect() && !blinky_m.justBeenKilled_m)
    target.draw(blinky_m);
  if (pinky_m.underBigGumEffect() && !pinky_m.justBeenKilled_m)
    target.draw(pinky_m);
  if (inky_m.underBigGumEffect() && !inky_m.justBeenKilled_m)
    target.draw(inky_m);
  if (clyde_m.underBigGumEffect() && !clyde_m.justBeenKilled_m)
    target.draw(clyde_m);
}

Preview::Preview(Pacman& pacman, Ghosts& ghosts, Bonuses& bonuses, Music& music)
    : CharAndNick_m("character / nickname", textScale_m, textScale_m,
                    charAndNickPosX_m, charAndNickPosY_m, false),
      shadow_m("-shadow", textScale_m, textScale_m, characterPosX_m,
               blinkyTextPosY_m, false, Blinky::getFillColor()),
      blinkyText_m("\"blinky\"", textScale_m, textScale_m, nicknamePosX_m,
                   blinkyTextPosY_m, false, Blinky::getFillColor()),
      speedy_m("-speedy", textScale_m, textScale_m, characterPosX_m,
               pinkyTextPosY_m, false, Pinky::getFillColor()),
      pinkyText_m("\"pinky\"", textScale_m, textScale_m, nicknamePosX_m,
                  pinkyTextPosY_m, false, Pinky::getFillColor()),
      bashful_m("-bashful", textScale_m, textScale_m, characterPosX_m,
                inkyTextPosY_m, false, Inky::getFillColor()),
      inkyText_m("\"inky\"", textScale_m, textScale_m, nicknamePosX_m,
                 inkyTextPosY_m, false, Inky::getFillColor()),
      pokey_m("-pokey", textScale_m, textScale_m, characterPosX_m,
              clydeTextPosY_m, false, Clyde::getFillColor()),
      clydeText_m("\"clyde\"", textScale_m, textScale_m, nicknamePosX_m,
                  clydeTextPosY_m, false, Clyde::getFillColor()),
      blinky_m(*ghosts.getGhostsTexture()),
      pinky_m(*ghosts.getGhostsTexture()),
      inky_m(*ghosts.getGhostsTexture()),
      clyde_m(*ghosts.getGhostsTexture()),
      bigGum_m(*bonuses.getGumsTexture(), BigGums::getTextureRect(),
               entityHbWidth),
      ten_m("10", textScale_m, textScale_m, numsPosX_m, gumPicPosY_m, false),
      fifty_m("50", textScale_m, textScale_m, numsPosX_m, bigGumPicPosY_m,
              false),
      author_m("2022 gostik", textScale_m, textScale_m, authorPosX_m,
               authorPosY_m, false),
      pressSpace_m("press space to continue", textScale_m, textScale_m,
                   pressSpacePosX_m, pressSpacePosY_m, true),
      munch_m(music.munchSound) {
  /// pictures
  blinkyPic_m.setTexture(*ghosts.getGhostsTexture());
  blinkyPic_m.setTextureRect(blinky_m.getTextureRect(Entity::Dir::Right));
  blinkyPic_m.setPosition(ghostPicturesPosX_m, blinkyPicPosY_m);

  pinkyPic_m.setTexture(*ghosts.getGhostsTexture());
  pinkyPic_m.setTextureRect(pinky_m.getTextureRect(Entity::Dir::Right));
  pinkyPic_m.setPosition(ghostPicturesPosX_m, pinkyPicPosY_m);

  inkyPic_m.setTexture(*ghosts.getGhostsTexture());
  inkyPic_m.setTextureRect(inky_m.getTextureRect(Entity::Dir::Right));
  inkyPic_m.setPosition(ghostPicturesPosX_m, inkyPicPosY_m);

  clydePic_m.setTexture(*ghosts.getGhostsTexture());
  clydePic_m.setTextureRect(clyde_m.getTextureRect(Entity::Dir::Right));
  clydePic_m.setPosition(ghostPicturesPosX_m, clydePicPosY_m);

  /// entities
  blinky_m.previewUpdateSpeed(ghostNormalSpeed_m, ghostFrightSpeed_m);
  blinky_m.calculateTimeConstants();

  pinky_m.previewUpdateSpeed(ghostNormalSpeed_m, ghostFrightSpeed_m);
  pinky_m.calculateTimeConstants();

  inky_m.previewUpdateSpeed(ghostNormalSpeed_m, ghostFrightSpeed_m);
  inky_m.calculateTimeConstants();

  clyde_m.previewUpdateSpeed(ghostNormalSpeed_m, ghostFrightSpeed_m);
  clyde_m.calculateTimeConstants();

  bigGum_m.setOrigin(bigGum_m.getSpriteLocalBounds().width / 2.0f,
                     bigGum_m.getSpriteLocalBounds().height / 2.0f);
  bigGum_m.setPosition(bigGumPosX_m, entityPosY_m);

  /// kill score
  killScoreSpr_m.setTexture(*ghosts.getKillScoreTexture());
  killScoreSpr_m.setTextureRect({killScoreStartRectL_m, killScoreStartRectT_m,
                                 entitySize_i, entitySize_i});
  killScoreSpr_m.setOrigin(killScoreSpr_m.getLocalBounds().width / 2.0f,
                           killScoreSpr_m.getGlobalBounds().height / 2.0f);
  killScoreSpr_m.setRotation(90.0f);

  /// gums pictures and relevant objects
  image_m.loadFromFile("../Images/preview16.png");
  image_m.createMaskFromColor(sf::Color::Black);
  texture_m.loadFromImage(image_m);

  gumPic_m.setTexture(*bonuses.getGumsTexture());
  gumPic_m.setTextureRect(Gums::getTextureRect());
  gumPic_m.setPosition(gumsPicPosX_m, gumPicPosY_m);
  pts1_m.setTexture(texture_m);
  pts1_m.setTextureRect({ptsRectL_m, ptsRectT_m, ptsWidth_m, ptsHeight_m});
  pts1_m.setPosition(ptsPosX_m, gumPicPosY_m);

  bigGumPic_m.setTexture(*bonuses.getGumsTexture());
  bigGumPic_m.setTextureRect(BigGums::getTextureRect());
  bigGumPic_m.setPosition(gumsPicPosX_m, bigGumPicPosY_m);
  pts2_m.setTexture(texture_m);
  pts2_m.setTextureRect({ptsRectL_m, ptsRectT_m, ptsWidth_m, ptsHeight_m});
  pts2_m.setPosition(ptsPosX_m, bigGumPicPosY_m);

  /// copyright symbol
  copyright_m.setTexture(texture_m);
  copyright_m.setTextureRect(
      {copyrightRectL_m, copyrightRectT_m, cellSize_i, cellSize_i});
  copyright_m.setPosition(copyrightPosX_m, copyrightPosY_m);
}

void Preview::update(float dTime) {
  if (startTurnOff_m) {
    if (munch_m.getStatus() == sf::SoundSource::Stopped) {
      accumTimePause_m += dTime;
      if (accumTimePause_m >= constTimeShortPause_m) isPlaying_m = false;
    }
  } else {
    accumTimePause_m += dTime;
    if (!firstIteration_m) {
      accumTimePressSpace_m += dTime;
      while (accumTimePressSpace_m >= constTimeShortPause_m) {
        accumTimePressSpace_m -= constTimeShortPause_m;
        showPressSpace_m = !showPressSpace_m;
      }
    }
    switch (state_m) {
      case start:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = blinkyPic;
        }
        break;
      case blinkyPic:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = shadow;
        }
        break;
      case shadow:
        if (accumTimePause_m >= constTimeShortPause_m) {
          accumTimePause_m -= constTimeShortPause_m;
          state_m = blinkyText;
        }
        break;
      case blinkyText:
        if (accumTimePause_m >= constTimeShortPause_m) {
          accumTimePause_m -= constTimeShortPause_m;
          state_m = pinkyPic;
        }
        break;
      case pinkyPic:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = speedy;
        }
        break;
      case speedy:
        if (accumTimePause_m >= constTimeShortPause_m) {
          accumTimePause_m -= constTimeShortPause_m;
          state_m = pinkyText;
        }
        break;
      case pinkyText:
        if (accumTimePause_m >= constTimeShortPause_m) {
          accumTimePause_m -= constTimeShortPause_m;
          state_m = inkyPic;
        }
        break;
      case inkyPic:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = bashful;
        }
        break;
      case bashful:
        if (accumTimePause_m >= constTimeShortPause_m) {
          accumTimePause_m -= constTimeShortPause_m;
          state_m = inkyText;
        }
        break;
      case inkyText:
        if (accumTimePause_m >= constTimeShortPause_m) {
          accumTimePause_m -= constTimeShortPause_m;
          state_m = clydePic;
        }
        break;
      case clydePic:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = pokey;
        }
        break;
      case pokey:
        if (accumTimePause_m >= constTimeShortPause_m) {
          accumTimePause_m -= constTimeShortPause_m;
          state_m = clydeText;
        }
        break;
      case clydeText:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = gums;
        }
        break;
      case gums:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = nickname;
        }
        break;
      case nickname:
        if (accumTimePause_m >= constTimeLongPause_m) {
          accumTimePause_m -= constTimeLongPause_m;
          state_m = move;
        }
        break;
      case move:
        if (someGhostWasKilled_m) {
          accumTimeGhostKill_m += dTime;
          if (accumTimeGhostKill_m >= constTimeGhostKill_m) {
            accumTimeGhostKill_m = 0;
            someGhostWasKilled_m = false;
            if (killScoreIndex_m == 4) {
              reload();
            }
          }
        } else {
          pacman_m.previewUpdate(dTime);
          blinky_m.previewUpdate(dTime);
          pinky_m.previewUpdate(dTime);
          inky_m.previewUpdate(dTime);
          clyde_m.previewUpdate(dTime);

          handleCollisionG(blinky_m);
          handleCollisionG(pinky_m);
          handleCollisionG(inky_m);
          handleCollisionG(clyde_m);
        }
        handleBigGums(dTime);
        break;
    }
  }
}

void Preview::turnOff() {
  if (!startTurnOff_m) {
    munch_m.play();
    startTurnOff_m = true;
    accumTimePause_m = 0;
  }
}

void Preview::reload() {
  pacman_m.previewSetStart(pacmanStartPosX_m, entityPosY_m);
  blinky_m.previewSetStart(blinkyStartPosX_m, entityPosY_m);
  pinky_m.previewSetStart(pinkyStartPosX_m, entityPosY_m);
  inky_m.previewSetStart(inkyStartPosX_m, entityPosY_m);
  clyde_m.previewSetStart(clydeStartPosX_m, entityPosY_m);

  state_m = start;

  killScoreIndex_m = 0;

  accumTimePause_m = 0;
  accumTimeGhostKill_m = 0;
  accumTimeShowBigGums_m = 0;

  someGhostWasKilled_m = false;
  bigGumWasEaten_m = false;
  showBigGums_m = true;
  firstIteration_m = false;
}

void Preview::reloadNewGame() {
  isPlaying_m = playPreview_b;
  if (isPlaying_m) {
    startTurnOff_m = false;
    showPressSpace_m = false;
    accumTimePressSpace_m = 0;
    reload();
    firstIteration_m = true;  // in reload function it = false
  }
}

bool Preview::isPlaying() const { return isPlaying_m; }

void Preview::handleBigGums(float dTime) {
  accumTimeShowBigGums_m += dTime;
  while (accumTimeShowBigGums_m >= constTimeShowBigGums_m) {
    accumTimeShowBigGums_m -= constTimeShowBigGums_m;
    showBigGums_m = !showBigGums_m;
  }
  if (!bigGumWasEaten_m) {
    if (pacman_m.getHitboxBounds().intersects(
            bigGum_m.getHitboxGlobalBounds())) {
      bigGumWasEaten_m = true;
      pacman_m.previewChangeBehaviour();
      blinky_m.previewChangeBehaviour();
      pinky_m.previewChangeBehaviour();
      inky_m.previewChangeBehaviour();
      clyde_m.previewChangeBehaviour();
    }
  }
}