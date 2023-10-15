#pragma once

#include <SFML/Audio.hpp>
#include <functional>
#include <iostream>
#include <vector>

#include "Globals.h"
#include "Music.h"

struct ClassicGameMusic {
  Music& music_m;

  sf::Sound deathEndSound;
  sf::Sound eatFruitSound;
  sf::Sound eatGhostSound;
  sf::Sound moveToCageSound;
  sf::Sound munchSound;

  sf::Sound bigGumEffectSound;
  sf::Sound deathStartSound;
  sf::Sound extraLifeSound;
  sf::Sound startGameSound;
  sf::Sound intermissionSound;

  // True bool-value means that sound is playing.
  // Bool-values need to pause and resume all playing sounds and contain
  // truthful information ONLY while calling according functions.
  std::vector<std::pair<std::reference_wrapper<sf::Sound>, bool>> sounds_m;
  std::vector<sf::Sound> sirens_m{5};
  int currSiren = 0;

 private:
  void createVectors();

 public:
  ClassicGameMusic(Music& music, int level);

  void pauseAllMusic();
  void resumeAllMusic();
  void stopAllMusic();

  void reloadNextLevel(int level);
  void reloadNewGame();
};