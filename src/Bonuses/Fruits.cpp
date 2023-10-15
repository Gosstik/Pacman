#include "Fruits.h"

void Fruits::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (fruitExists_m)
    target.draw(fruitsMySpr_m);
  else if (fruitScoreExists_m)
    target.draw(fruitScoreSpr_m);
}

Fruits::Fruits(sf::Texture& fruitsTex, sf::Texture& fruitsScoreTex, int level)
    : fruitsMySpr_m(fruitsTex, {0, 0, entitySize_i, entitySize_i}, hbWidth_m),
      gen_m(seed_m()) {
  fruitsMySpr_m.setOrigin(fruitsMySpr_m.getSpriteLocalBounds().width / 2.0f,
                          fruitsMySpr_m.getSpriteLocalBounds().height / 2.0f);
  fruitsMySpr_m.setPosition(fruitPosPixX_m, fruitPosPixY_m);
  fruits_m.reserve(8);

  fruits_m.push_back({100, 1, firstFruitRectLeft_m, firstFruitRectTop_m});
  fruits_m.push_back(
      {300, 2, firstFruitRectLeft_m + entitySize_i, firstFruitRectTop_m});
  fruits_m.push_back(
      {500, 3, firstFruitRectLeft_m + entitySize_i * 2, firstFruitRectTop_m});
  fruits_m.push_back(
      {700, 5, firstFruitRectLeft_m + entitySize_i * 3, firstFruitRectTop_m});
  fruits_m.push_back(
      {1000, 7, firstFruitRectLeft_m + entitySize_i * 4, firstFruitRectTop_m});
  fruits_m.push_back(
      {2000, 9, firstFruitRectLeft_m + entitySize_i * 5, firstFruitRectTop_m});
  fruits_m.push_back(
      {3000, 11, firstFruitRectLeft_m + entitySize_i * 6, firstFruitRectTop_m});
  fruits_m.push_back(
      {5000, 13, firstFruitRectLeft_m + entitySize_i * 7, firstFruitRectTop_m});

  curr_m = 0;
  fruitsMySpr_m.setTextureRect({fruits_m[curr_m].rectLeft,
                                fruits_m[curr_m].rectTop, entitySize_i,
                                entitySize_i});
  while (curr_m < fruits_m.size() - 1 && level > fruits_m[curr_m].minLevel) {
    ++curr_m;
    fruitsMySpr_m.setTextureRect({fruits_m[curr_m].rectLeft,
                                  fruits_m[curr_m].rectTop, entitySize_i,
                                  entitySize_i});
  }

  fruitScoreSpr_m.setTexture(fruitsScoreTex);
  fruitScoreSpr_m.setTextureRect(
      {curr_m * entitySize_i, 0, entitySize_i, fruitScoreRectHeight_m});
  fruitScoreSpr_m.setOrigin(fruitScoreSpr_m.getLocalBounds().width / 2.0f,
                            fruitScoreSpr_m.getLocalBounds().height / 2.0f);
  fruitScoreSpr_m.setRotation(90);
  fruitScoreSpr_m.setPosition(fruitPosPixX_m, fruitPosPixY_m);
}

void Fruits::update(Pacman& pacman, Ghosts& ghosts, Score& score, Gums& gums,
                    ClassicGameMusic& CGMusic, bool& pacmanAteBonus,
                    float dTime) {
  if (!firstAppeared_m && gums.count() <= firstBorder_m) {
    firstAppeared_m = true;
    constTimeFruitExists_m =
        minTimeFruitExists_m + float(gen_m() % 100) * 10000;
    fruitExists_m = true;
  } else if (!secondAppeared_m && gums.count() <= secondBorder_m) {
    secondAppeared_m = true;
    constTimeFruitExists_m =
        minTimeFruitExists_m + float(gen_m() % 100) * 10000;
    fruitExists_m = true;
  } else if (fruitExists_m) {
    accumTimeFruitExists_m += dTime;
    if (pacman.getHitboxBounds().intersects(
            fruitsMySpr_m.getHitboxGlobalBounds())) {
      // music
      pacmanAteBonus = true;
      CGMusic.eatFruitSound.play();

      // other
      accumTimeFruitExists_m = 0;
      fruitExists_m = false;
      fruitScoreExists_m = true;
      score.add(fruits_m[curr_m].cost);
      ghosts.pacmanAteBonus();
    } else if (accumTimeFruitExists_m >= constTimeFruitExists_m) {
      accumTimeFruitExists_m = 0;
      fruitExists_m = false;
    }
  } else if (fruitScoreExists_m) {
    accumTimeFruitScoreExists_m += dTime;
    if (accumTimeFruitScoreExists_m >= constTimeFruitScoreExists_m) {
      accumTimeFruitScoreExists_m = 0;
      fruitScoreExists_m = false;
    }
  }
}

const sf::IntRect& Fruits::getTextureRect() const {
  return fruitsMySpr_m.getTextureRect();
}

void Fruits::reloadAfterPacmanDeath(Map& map) {
  fruitExists_m = fruitScoreExists_m = false;
  accumTimeFruitScoreExists_m = accumTimeFruitExists_m =
      constTimeFruitExists_m = 0;
}

void Fruits::reloadNextLevel(int level) {
  if (curr_m < fruits_m.size() - 1 && level > fruits_m[curr_m].minLevel) {
    ++curr_m;
    fruitsMySpr_m.setTextureRect({fruits_m[curr_m].rectLeft,
                                  fruits_m[curr_m].rectTop, entitySize_i,
                                  entitySize_i});
    fruitScoreSpr_m.setTextureRect(
        {curr_m * entitySize_i, 0, entitySize_i, fruitScoreRectHeight_m});
  }
  firstAppeared_m = secondAppeared_m = fruitExists_m = fruitScoreExists_m =
      false;
  accumTimeFruitScoreExists_m = accumTimeFruitExists_m =
      constTimeFruitExists_m = 0;
}

void Fruits::reloadNewGame() {
  curr_m = 0;
  fruitsMySpr_m.setTextureRect({fruits_m[curr_m].rectLeft,
                                fruits_m[curr_m].rectTop, entitySize_i,
                                entitySize_i});
  fruitScoreSpr_m.setTextureRect(
      {curr_m * entitySize_i, 0, entitySize_i, fruitScoreRectHeight_m});

  firstAppeared_m = secondAppeared_m = fruitExists_m = fruitScoreExists_m =
      false;
  accumTimeFruitScoreExists_m = accumTimeFruitExists_m =
      constTimeFruitExists_m = 0;
}