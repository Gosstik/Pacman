#include "Bonuses.h"

void Bonuses::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(gums_m);
  target.draw(bigGums_m);
  target.draw(fruits_m);
}

Bonuses::Bonuses(Map& map, Pacman& pacman, int level)
    : gums_m(gumsTex_m, map),
      bigGums_m(gumsTex_m),
      fruits_m(fruitsTex_m, fruitScoreTex_m, level) {
  fruitsImg_m.loadFromFile(fruitsFilename_m);
  fruitsImg_m.createMaskFromColor(sf::Color::Black);
  fruitsTex_m.loadFromImage(fruitsImg_m);

  gumsImg_m.loadFromFile(gumsFilename_m);
  gumsImg_m.createMaskFromColor(sf::Color::Black);
  gumsTex_m.loadFromImage(gumsImg_m);

  fruitScoreImg_m.loadFromFile(fruitScoreFilename_m);
  fruitScoreImg_m.createMaskFromColor(sf::Color::Black);
  fruitScoreTex_m.loadFromImage(fruitScoreImg_m);
}

void Bonuses::update(Map& map, Pacman& pacman, Ghosts& ghosts, Score& score,
                     ClassicGameMusic& CGMusic, bool frightMode, float dTime) {
  gums_m.update(pacman, ghosts, score, CGMusic, pacmanAteBonus_m);
  bigGums_m.update(map, pacman, ghosts, score, CGMusic, pacmanAteBonus_m,
                   dTime);
  fruits_m.update(pacman, ghosts, score, gums_m, CGMusic, pacmanAteBonus_m,
                  dTime);

  handlePacmanMunchAndSpeed(pacman, ghosts, CGMusic, frightMode);
}

void Bonuses::handlePacmanMunchAndSpeed(Pacman& pacman, Ghosts& ghosts,
                                        ClassicGameMusic& CGMusic,
                                        bool frightMode) {
  if (pacmanAteBonus_m) {
    if (CGMusic.munchSound.getStatus() != sf::SoundSource::Playing) {
      CGMusic.munchSound.setLoop(true);
      CGMusic.munchSound.play();
      pacman.setDotsSpeed(frightMode);
    }
    accumPacmanCoveredDistOneIter_m = 0;
    ghosts.resetCageAccumTime();
  }

  if (CGMusic.munchSound.getLoop() && !pacmanAteBonus_m) {
    accumPacmanCoveredDistOneIter_m += pacman.getCoveredDistOneIter();
    if (accumPacmanCoveredDistOneIter_m > cellSize_f + 1 ||
        pacman.getCurrDir() == Entity::Dir::Stop) {
      CGMusic.munchSound.setLoop(false);
      accumPacmanCoveredDistOneIter_m = 0;
      pacman.setNormalSpeed(frightMode);
    }
  }
  pacmanAteBonus_m = false;
}

void Bonuses::showBigGums() { bigGums_m.show(); }

const sf::Texture* Bonuses::getGumsTexture() const { return &gumsTex_m; }

const sf::Texture* Bonuses::getFruitsTexture() const { return &fruitsTex_m; }

const sf::IntRect& Bonuses::getFruitTextureRect() const {
  return fruits_m.getTextureRect();
}

int Bonuses::gumsLeft() const { return gums_m.count(); }

int Bonuses::bigGumsLeft() const { return bigGums_m.count(); }

int Bonuses::allGumsLeft() const { return gums_m.count() + bigGums_m.count(); }

void Bonuses::reloadAfterPacmanDeath(Map& map) {
  gums_m.reloadAfterPacmanDeath(map);
  bigGums_m.reloadAfterPacmanDeath();
  fruits_m.reloadAfterPacmanDeath(map);
  accumPacmanCoveredDistOneIter_m = 0;
  pacmanAteBonus_m = false;
}

void Bonuses::reloadNextLevel(Map& map, int level) {
  gums_m.reloadNextLevel(map);
  bigGums_m.reloadNextLevel();
  fruits_m.reloadNextLevel(level);
  accumPacmanCoveredDistOneIter_m = 0;
  pacmanAteBonus_m = false;
}

void Bonuses::reloadNewGame(Map& map) {
  gums_m.reloadNewGame(map);
  bigGums_m.reloadNewGame();
  fruits_m.reloadNewGame();
  accumPacmanCoveredDistOneIter_m = 0;
  pacmanAteBonus_m = false;
}