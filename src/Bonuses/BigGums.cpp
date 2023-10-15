#include "BigGums.h"

void BigGums::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (showBigGums_m)
    for (auto& bigGum : bigGums_m) target.draw(bigGum);
}

BigGums::BigGums(sf::Texture& texture)
    : mySprite_m(texture, {rectLeft_m, rectTop_m, cellSize_i, cellSize_i},
                 hbWidth_m) {
  mySprite_m.setOrigin(mySprite_m.getSpriteLocalBounds().width / 2.0f,
                       mySprite_m.getSpriteLocalBounds().height / 2.0f);
  create();
}

void BigGums::create() {
  bigGums_m.resize(4);
  bigGums_m[0] = mySprite_m;
  bigGums_m[0].setPosition(bigGumLeftTopPixX_m, bigGumLeftTopPixY_m);
  bigGums_m[1] = mySprite_m;
  bigGums_m[1].setPosition(bigGumRightTopPixX_m, bigGumRightTopPixY_m);
  bigGums_m[2] = mySprite_m;
  bigGums_m[2].setPosition(bigGumLeftBottomPixX_m, bigGumLeftBottomPixY_m);
  bigGums_m[3] = mySprite_m;
  bigGums_m[3].setPosition(bigGumRightBottomPixX_m, bigGumRightBottomPixY_m);
}

void BigGums::update(Map& map, Pacman& pacman, Ghosts& ghosts, Score& score,
                     ClassicGameMusic& CGMusic, bool& pacmanAteBonus,
                     float dTime) {
  accumTimeShowBigGums_m += dTime;
  if (accumTimeShowBigGums_m >= constTimeShowBigGums_m) {
    while (accumTimeShowBigGums_m >= constTimeShowBigGums_m) {
      accumTimeShowBigGums_m -= constTimeShowBigGums_m;
      showBigGums_m = !showBigGums_m;
    }
  }
  auto iter = std::find_if(bigGums_m.begin(), bigGums_m.end(),
                           [&pacman](Sprite1H& mySprite) {
                             return mySprite.getHitboxGlobalBounds().intersects(
                                 pacman.getHitboxBounds());
                           });
  if (iter != bigGums_m.end()) {
    // music
    pacmanAteBonus = true;

    // other
    bigGums_m.erase(iter);
    score.add(cost_m);
    ghosts.setBigGumEffect(map, CGMusic);
    ghosts.pacmanAteBonus();
  }
}

void BigGums::show() { showBigGums_m = true; }

int BigGums::count() const { return static_cast<int>(bigGums_m.size()); }

sf::IntRect BigGums::getTextureRect() {
  return {rectLeft_m, rectTop_m, cellSize_i, cellSize_i};
}

void BigGums::reloadAfterPacmanDeath() {
  accumTimeShowBigGums_m = 0;
  showBigGums_m = true;
}

void BigGums::reloadNextLevel() {
  if (!bigGums_m.empty()) {
    std::cerr << "BigGums::reloadNextLevel : !bigGums_m.empty()\n";
  }
  bigGums_m.clear();
  create();
  accumTimeShowBigGums_m = 0;
  showBigGums_m = true;
}

void BigGums::reloadNewGame() {
  bigGums_m.clear();
  create();
  accumTimeShowBigGums_m = 0;
  showBigGums_m = true;
}