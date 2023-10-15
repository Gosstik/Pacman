#include "Globals.h"

/// --------------------------------------------
/// common
const sf::Color entityHbColor = {0, 255, 0};
const sf::Color entityWallsHbColor = {139, 0, 255};
const std::string blackSquare16ImageFilename_m = "../Images/blackSquare16.png";
const sf::Color darkeningColor = sf::Color(255, 255, 255, 50);

CurrMode currMode;

/// --------------------------------------------
/// settings
bool gameFirstOpen;  // defined in class FirstOpen

bool showFPS_b = false;
bool pauseBeforeContinueGame_b = true;
bool showCountdown_b = true;
bool showLevel_b = true;
bool playMusicInMM_b = true;
bool simplifiedTurns_b = true;
bool playPreview_b = true;

bool showCycles_b = false;
bool showEntityHitbox_b = false;
bool showButtonsHitbox_b = false;
bool showGhostsTargets_b = false;

/// --------------------------------------------
/// Window

/// --------------------------------------------
/// Map

/// --------------------------------------------
/// Portals

/// --------------------------------------------
/// FPS

/// --------------------------------------------
/// MyText

/// --------------------------------------------
/// Entity

/// --------------------------------------------
/// Pacman

/// --------------------------------------------
/// Ghosts

/// --------------------------------------------
/// Bonuses

/// --------------------------------------------
/// Scenes
bool event_MainMenu = true;
bool event_Close = false;
bool event_FullClose = false;
bool event_ChooseMode = false;
bool event_ClassicGame = false;
bool event_EndlessMap = false;
bool event_StartGame = false;
bool event_Pause = false;
bool event_MainMenuSettings = false;
bool event_PauseSettings = false;
bool event_BackMainMenu = false;
bool event_GameOver = false;
bool event_NewGame = true;  /// change behaviour!!!!!!!!!!!!!!!!!
bool event_Profiles = false;
bool event_InDevelopment = false;

bool calledFromGameMode = false;
bool calledFromPause = false;
bool calledByExitButton = false;

template <class T>
T abs(T elem) {
  return (elem < 0) ? -elem : elem;
}
template float abs<float>(float elem);

bool empty(std::fstream& flow) {
  return flow.peek() == std::fstream::traits_type::eof();
}