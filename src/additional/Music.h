#pragma once

#include <SFML/Audio.hpp>
#include <functional>
#include <vector>

#include "Globals.h"

class Music {
 public:
  enum SoundKind { music, effect };

 private:
  std::vector<std::reference_wrapper<sf::Sound>> musicVec_m;
  std::vector<std::reference_wrapper<sf::Sound>> effectVec_m;
  float musicVolume_m = 100.0f;
  float effectVolume_m = 100.0f;

 public:
  // button
  sf::SoundBuffer hoverSoundBuf;
  sf::SoundBuffer pressSoundBuf;
  sf::SoundBuffer releaseSoundBuf;
  sf::SoundBuffer backSoundBuf;
  sf::SoundBuffer setPopUpWindowBuf;

  // classic game
  sf::SoundBuffer deathEndSoundBuf;
  sf::SoundBuffer eatFruitSoundBuf;
  sf::SoundBuffer eatGhostSoundBuf;
  sf::SoundBuffer moveToCageSoundBuf;
  sf::SoundBuffer munchS80SoundBuf;
  sf::SoundBuffer munchS90SoundBuf;
  sf::SoundBuffer munchS100SoundBuf;
  sf::SoundBuffer siren1SoundBuf;
  sf::SoundBuffer siren2SoundBuf;
  sf::SoundBuffer siren3SoundBuf;
  sf::SoundBuffer siren4SoundBuf;
  sf::SoundBuffer siren5SoundBuf;

  sf::SoundBuffer bigGumEffectSoundBuf;
  sf::SoundBuffer deathStartSoundBuf;
  sf::SoundBuffer extraLifeSoundBuf;
  sf::SoundBuffer startGameSoundBuf;
  sf::SoundBuffer intermissionSoundBuf;

  sf::Sound backSound;
  sf::Sound hoverSound;
  sf::Sound pressSound;
  sf::Sound releaseSound;
  sf::Sound setPopUpWindowSound;

  sf::Sound MMSoundtrack;
  sf::Sound munchSound;

  Music();
  void addSound(sf::Sound& sound, const SoundKind& soundKind);
  void createSound(sf::Sound& sound, sf::SoundBuffer& soundBuf,
                   bool loop = false,
                   const SoundKind& soundKind = Music::SoundKind::music);
  void setMusicVolume(float volume);
  void setEffectVolume(float volume);
  void setVolume(const Music::SoundKind& soundKind, float volume);

  float getMusicVolume() const;
  float getEffectVolume() const;

  static float getIntermissionDuration();
};