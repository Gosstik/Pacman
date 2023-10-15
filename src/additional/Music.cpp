#include "Music.h"

Music::Music() {
  hoverSoundBuf.loadFromFile("../Audio/Button/hover.wav");
  pressSoundBuf.loadFromFile("../Audio/Button/press.wav");
  releaseSoundBuf.loadFromFile("../Audio/Button/release.wav");
  backSoundBuf.loadFromFile("../Audio/Button/back.wav");
  setPopUpWindowBuf.loadFromFile("../Audio/Button/setPopUpWindow.wav");

  deathEndSoundBuf.loadFromFile("../Audio/ClassicGame/deathEnd.wav");
  eatFruitSoundBuf.loadFromFile("../Audio/ClassicGame/eatFruit.wav");
  eatGhostSoundBuf.loadFromFile("../Audio/ClassicGame/eatGhost.wav");
  moveToCageSoundBuf.loadFromFile("../Audio/ClassicGame/moveToCage.wav");
  munchS80SoundBuf.loadFromFile("../Audio/ClassicGame/munchS80.wav");
  munchS90SoundBuf.loadFromFile("../Audio/ClassicGame/munchS90.wav");
  munchS100SoundBuf.loadFromFile("../Audio/ClassicGame/munchS100.wav");
  siren1SoundBuf.loadFromFile("../Audio/ClassicGame/siren1.wav");
  siren2SoundBuf.loadFromFile("../Audio/ClassicGame/siren2.wav");
  siren3SoundBuf.loadFromFile("../Audio/ClassicGame/siren3.wav");
  siren4SoundBuf.loadFromFile("../Audio/ClassicGame/siren4.wav");
  siren5SoundBuf.loadFromFile("../Audio/ClassicGame/siren5.wav");

  bigGumEffectSoundBuf.loadFromFile("../Audio/ClassicGame/bigGumEffect.wav");
  deathStartSoundBuf.loadFromFile("../Audio/ClassicGame/deathStart.wav");
  extraLifeSoundBuf.loadFromFile("../Audio/ClassicGame/extraLife.wav");
  startGameSoundBuf.loadFromFile("../Audio/ClassicGame/startGame.wav");
  intermissionSoundBuf.loadFromFile("../Audio/ClassicGame/intermission.wav");

  createSound(backSound, backSoundBuf, false, Music::SoundKind::effect);
  createSound(hoverSound, hoverSoundBuf, false, Music::SoundKind::effect);
  createSound(pressSound, pressSoundBuf, false, Music::SoundKind::effect);
  createSound(releaseSound, releaseSoundBuf, false, Music::SoundKind::effect);
  createSound(setPopUpWindowSound, setPopUpWindowBuf, false,
              Music::SoundKind::effect);

  createSound(MMSoundtrack, intermissionSoundBuf, true);
  createSound(munchSound, munchS80SoundBuf, false);

  setMusicVolume(musicVolume_m);
  setEffectVolume(effectVolume_m);
}

void Music::addSound(sf::Sound& sound, const SoundKind& soundKind) {
  if (soundKind == Music::SoundKind::music)
    musicVec_m.emplace_back(sound);
  else
    effectVec_m.emplace_back(sound);
}

void Music::createSound(sf::Sound& sound, sf::SoundBuffer& soundBuf, bool loop,
                        const SoundKind& soundKind) {
  sound.setBuffer(soundBuf);
  sound.setLoop(loop);
  addSound(sound, soundKind);
  if (soundKind == Music::SoundKind::music) {
    sound.setVolume(musicVolume_m);
  } else
    sound.setVolume(effectVolume_m);
}

void Music::setMusicVolume(float volume) {
  musicVolume_m = volume;
  for (auto& sound : musicVec_m) sound.get().setVolume(musicVolume_m);
}

void Music::setEffectVolume(float volume) {
  effectVolume_m = volume;
  for (auto& sound : effectVec_m) sound.get().setVolume(effectVolume_m);
}

float Music::getMusicVolume() const { return musicVolume_m; }

void Music::setVolume(const Music::SoundKind& soundKind, float volume) {
  if (soundKind == Music::SoundKind::music) {
    musicVolume_m = volume;
    for (auto& sound : musicVec_m) sound.get().setVolume(musicVolume_m);
  } else {
    effectVolume_m = volume;
    for (auto& sound : effectVec_m) sound.get().setVolume(effectVolume_m);
  }
}

float Music::getEffectVolume() const { return effectVolume_m; }

float Music::getIntermissionDuration() {
  sf::Music inter;
  inter.openFromFile("../Audio/ClassicGame/intermission.wav");
  return float(inter.getDuration().asMicroseconds());
}