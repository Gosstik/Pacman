#include "MusicOptions.h"

void MusicOptions::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  target.draw(musicVolume_m);
  target.draw(effectsVolume_m);
  target.draw(playMusicInMM_m);
}

MusicOptions::MusicOptions(Music& music)
    : MMSoundtrack_m(music.MMSoundtrack),
      musicVolume_m(music, musicVolumeScale_m, MMOptionNamePosX,
                    musicVolumeNamePosY_m, MMExtremeRightPosX),
      effectsVolume_m(music, effectsVolumeScale_m, MMOptionNamePosX,
                      effectsVolumeNamePosY_m, MMExtremeRightPosX),
      playMusicInMM_m(music, "play music in main menu", playMusicInMMPosY_m,
                      playMusicInMM_b) {}

void MusicOptions::handleEvents(sf::Event& event, sf::Vector2f& mousePos) {
  musicVolume_m.handleEvents(event, mousePos);
  effectsVolume_m.handleEvents(event, mousePos);
  playMusicInMM_m.handleEvents(event, mousePos);
  if (!calledFromPause) {
    if (playMusicInMM_b &&
        MMSoundtrack_m.getStatus() != sf::SoundSource::Playing)
      MMSoundtrack_m.play();
    else if (!playMusicInMM_b &&
             MMSoundtrack_m.getStatus() == sf::SoundSource::Playing)
      MMSoundtrack_m.stop();
  }
}

void MusicOptions::reloadAfterOpenScene(sf::Vector2f& mousePos,
                                        bool wasReleased) {
  musicVolume_m.reloadAfterOpenScene(mousePos, wasReleased);
  effectsVolume_m.reloadAfterOpenScene(mousePos, wasReleased);
  playMusicInMM_m.reloadAfterOpenScene(mousePos, wasReleased);
}

void MusicOptions::saveData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::out |
                                          std::fstream::trunc);

  stream.write(reinterpret_cast<char*>(&playMusicInMM_b),
               sizeof(playMusicInMM_b));
  musicVolume_m.saveData(stream);
  effectsVolume_m.saveData(stream);
}

void MusicOptions::readData() {
  std::fstream stream(saveFilename_m, std::fstream::binary | std::fstream::in);

  stream.read(reinterpret_cast<char*>(&playMusicInMM_b),
              sizeof(playMusicInMM_b));
  musicVolume_m.readData(stream);
  effectsVolume_m.readData(stream);
}