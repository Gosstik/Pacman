#include "Gums.h"

void Gums::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto& line : gums_m)
    for (auto&& gum : line) target.draw(gum);
}

Gums::Gums(sf::Texture& texture, Map& map)
    : mySprite_m(texture, {rectLeft_m, rectTop_m, cellSize_i, cellSize_i},
                 hbWidth_m) {
  mySprite_m.setOrigin(mySprite_m.getSpriteLocalBounds().width / 2.0f,
                       mySprite_m.getSpriteLocalBounds().height / 2.0f);
  create(map);
}

void Gums::create(Map& map) {
  gums_m.resize(mapWidthCell);
  for (int y = 0; y < mapHeightCell; ++y)
    for (int x = 0; x < mapWidthCell; ++x)
      if (map.getCellState(x, y) == 'S') {
        gums_m[x].emplace_back(mySprite_m);
        gums_m[x].back().setPosition(
            (float(x + mapOffsetCellX_i) + 0.5f) * cellSize_f,
            (float(y + mapOffsetCellY_i) + 0.5f) * cellSize_f);
      }
}

void Gums::update(Pacman& pacman, Ghosts& ghosts, Score& score,
                  ClassicGameMusic& CGMusic, bool& pacmanAteBonus) {
  int pacmanX = pacman.getMapPosition().x;
  for (int x = std::max(0, pacmanX - 1);
       x < std::min(int(gums_m.size()), pacmanX + 1); ++x)
    for (auto iter = gums_m[x].begin(); iter < gums_m[x].end(); ++iter)
      if ((*iter).getHitboxGlobalBounds().intersects(
              pacman.getHitboxBounds())) {
        // music
        pacmanAteBonus = true;

        // other
        gums_m[x].erase(iter);
        score.add(cost_m);
        ghosts.pacmanAteBonus();
        return;
      }
}

int Gums::count() const {
  size_t res = 0;
  for (auto& line : gums_m) res += line.size();
  return static_cast<int>(res);
}

sf::IntRect Gums::getTextureRect() {
  return {rectLeft_m, rectTop_m, cellSize_i, cellSize_i};
}

void Gums::reloadAfterPacmanDeath(Map& map) {}

void Gums::reloadNextLevel(Map& map) {
  if (count() != 0) {
    for (auto& line : gums_m) line.clear();
    std::cerr << "Gums::reloadNextLevel : count() != 0\n";
  }
  create(map);
}

void Gums::reloadNewGame(Map& map) {
  for (auto& line : gums_m) line.clear();
  create(map);
}