#pragma once

#include <functional>
#include <memory>
#include <stack>

#include "Bonuses.h"
#include "ClassicGameLabels.h"
#include "ClassicGameMusic.h"
#include "Ghosts.h"
#include "Intermissions.h"
#include "Map.h"
#include "MyWindow.h"
#include "Pacman.h"
#include "Portals.h"
#include "Preview.h"
#include "Score.h"
#include "TechInfo.h"

class ClassicGame {
  int level_m = 1;
  ClassicGameMusic CGMusic_m;

  Map map_m;
  Portals portals_m;
  Pacman pacman_m;
  Ghosts ghosts_m;
  Score score_m;
  Bonuses bonuses_m;

  ClassicGameLabels CGLabels_m;

  /// additional
  Intermissions intermissions_m;
  Preview preview_m;

  int intermissionFrequency_m = 1;

  sf::Event event_m{};
  float dTime_m = 0;
  float dTimeAccum_m = 0;

  bool pauseBeforeKillAnimation_m = false;
  bool pauseAfterKillAnimation_m = false;
  bool pauseBeforeEndLevelAnimation = false;
  bool executePacmanKillAnimation_m = false;
  bool endLevel_m = false;
  bool someGhostWasKilled_m = false;

 private:
  void reloadAfterOpen(MyWindow& window, TechInfo& techInfo);
  void handleEvents(MyWindow& window,
                    std::stack<std::reference_wrapper<bool>>& prevScene);
  template <class T>
  void displayObjects(T& window, TechInfo& techInfo, bool makeScreenshot);
  void makeScreenshot(MyWindow& window, TechInfo& techInfo);

  void setDefaultVariables();
  void setGameOver();
  void setEndLevel();

  void handleExtraLife();
  void handleGumsCount();

  void updateIntermissionFrequency();

 public:
  explicit ClassicGame(Music& music);
  void open(MyWindow& window,
            std::stack<std::reference_wrapper<bool>>& prevScene,
            TechInfo& techInfo);

  void reloadAfterPacmanDeath();
  void reloadNextLevel();
  void reloadNewGame();

  void saveData();
};