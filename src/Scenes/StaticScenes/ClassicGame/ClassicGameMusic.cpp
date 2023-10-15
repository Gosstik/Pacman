#include "ClassicGameMusic.h"

void ClassicGameMusic::createVectors() {
  sounds_m.emplace_back(deathEndSound, false);
  sounds_m.emplace_back(eatFruitSound, false);
  sounds_m.emplace_back(eatGhostSound, false);
  sounds_m.emplace_back(moveToCageSound, false);
  sounds_m.emplace_back(munchSound, false);
  sounds_m.emplace_back(sirens_m[0], false);
  sounds_m.emplace_back(sirens_m[1], false);
  sounds_m.emplace_back(sirens_m[2], false);
  sounds_m.emplace_back(sirens_m[3], false);
  sounds_m.emplace_back(sirens_m[4], false);

  sounds_m.emplace_back(bigGumEffectSound, false);
  sounds_m.emplace_back(deathStartSound, false);
  sounds_m.emplace_back(extraLifeSound, false);
  sounds_m.emplace_back(startGameSound, false);
  sounds_m.emplace_back(intermissionSound, false);
}

ClassicGameMusic::ClassicGameMusic(Music& music, int level) : music_m(music) {
  createVectors();

  // sound effect
  music.createSound(deathEndSound, music.deathEndSoundBuf);
  music.createSound(eatFruitSound, music.eatFruitSoundBuf);
  music.createSound(eatGhostSound, music.eatGhostSoundBuf);
  music.createSound(moveToCageSound, music.moveToCageSoundBuf, true);

  if (level == 1)
    music.createSound(munchSound, music.munchS80SoundBuf, true);
  else if (level < 5 || level > 20)
    music.createSound(munchSound, music.munchS90SoundBuf, true);
  else
    music.createSound(munchSound, music.munchS100SoundBuf, true);

  music.createSound(sirens_m[0], music.siren1SoundBuf, true);
  music.createSound(sirens_m[1], music.siren2SoundBuf, true);
  music.createSound(sirens_m[2], music.siren3SoundBuf, true);
  music.createSound(sirens_m[3], music.siren4SoundBuf, true);
  music.createSound(sirens_m[4], music.siren5SoundBuf, true);

  // music
  music.createSound(bigGumEffectSound, music.bigGumEffectSoundBuf, true);
  music.createSound(deathStartSound, music.deathStartSoundBuf);
  music.createSound(extraLifeSound, music.extraLifeSoundBuf);
  music.createSound(startGameSound, music.startGameSoundBuf);
  music.createSound(intermissionSound, music.intermissionSoundBuf, true);
}

void ClassicGameMusic::pauseAllMusic() {
  for (auto& elem : sounds_m) {
    if (elem.first.get().getStatus() == sf::SoundSource::Playing) {
      elem.first.get().pause();
      elem.second = true;
    } else
      elem.second = false;
  }
}

void ClassicGameMusic::resumeAllMusic() {
  for (auto& elem : sounds_m)
    if (elem.second) elem.first.get().play();
}

void ClassicGameMusic::stopAllMusic() {
  for (auto& elem : sounds_m) elem.first.get().stop();
}

void ClassicGameMusic::reloadNextLevel(int level) {
  // munch
  if (level == 1)
    munchSound.setBuffer(music_m.munchS80SoundBuf);
  else if (level == 2 || level == 21)
    munchSound.setBuffer(music_m.munchS90SoundBuf);
  else if (level == 5)
    munchSound.setBuffer(music_m.munchS100SoundBuf);

  // siren
  currSiren = 0;
}

void ClassicGameMusic::reloadNewGame() {
  startGameSound.stop();
  //        startGameSound.play();
  munchSound.setBuffer(music_m.munchS80SoundBuf);
  currSiren = 0;
}