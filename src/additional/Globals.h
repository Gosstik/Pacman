#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

/// --------------------------------------------
/// common
static constexpr float pi = 3.1415926535f;
static constexpr int cellSize_i = 16;
static constexpr float cellSize_f = float(cellSize_i);
static constexpr int entitySize_i = cellSize_i * 2;
static constexpr float entitySize_f = cellSize_f * 2;
static constexpr float mcsInSec = 1000000.0f;
static constexpr int pauseBeforeContinueLevel = 2;  // seconds
static constexpr int pauseBeforeNewGame = 5;        // seconds
static constexpr int scoreForExtraLife = 10000;
extern const sf::Color entityHbColor;
extern const sf::Color entityWallsHbColor;
extern const std::string blackSquare16ImageFilename_m;
extern const sf::Color darkeningColor;

struct CurrMode {
  enum Mode { classicGame = 0, endlessMap };
  enum Up { one = 0, two, network };
  Mode mode = classicGame;
  Up up = one;
};
extern CurrMode currMode;

/// --------------------------------------------
/// settings
extern bool gameFirstOpen;

extern bool showFPS_b;
extern bool pauseBeforeContinueGame_b;
extern bool showCountdown_b;
extern bool showLevel_b;
extern bool playMusicInMM_b;
extern bool simplifiedTurns_b;
extern bool playPreview_b;

extern bool showCycles_b;
extern bool showEntityHitbox_b;
extern bool showButtonsHitbox_b;
extern bool showGhostsTargets_b;

/// --------------------------------------------
/// Window
static constexpr int viewHeightPix_i = cellSize_i * 36;
static constexpr int viewWidthPix_i = cellSize_i * 36;
static constexpr float viewHeightPix_f = float(viewHeightPix_i);
static constexpr float viewWidthPix_f = float(viewWidthPix_i);

/// --------------------------------------------
/// Map
static constexpr int mapHeightCell = 31;
static constexpr int mapWidthCell = 32;
static constexpr int mapHeightPix = mapHeightCell * cellSize_i;
static constexpr int mapWidthPix = mapWidthCell * cellSize_i;
static constexpr int mapOffsetCellX_i = 2;
static constexpr int mapOffsetCellY_i = 3;
static constexpr float mapOffsetCellX_f = 2.0f;
static constexpr float mapOffsetCellY_f = 3.0f;

/// --------------------------------------------
/// Portals
static constexpr float leftPortalCenterPixX =
    (mapOffsetCellX_f + 0.5f) * cellSize_f;
static constexpr float leftPortalCenterPixY =
    (mapOffsetCellY_f + 14.5f) * cellSize_f;
static constexpr float rightPortalCenterPixX =
    leftPortalCenterPixX + (mapWidthCell - 1) * cellSize_f;
static constexpr float rightPortalCenterPixY = leftPortalCenterPixY;

/// --------------------------------------------
/// FPS

/// --------------------------------------------
/// MyText

/// --------------------------------------------
/// Entity
static constexpr float maxSpeed =
    (cellSize_f / 8.0f) *
    0.0000757576f;  // 0.00005f; // speed * dTime = num of pixels to move
static constexpr float entityHbWidth = cellSize_f / 2;
static constexpr float entityWallsHbWidth = cellSize_f;
static constexpr float hbThickness = 10;

/// --------------------------------------------
/// Pacman
static constexpr int pacmanStartNumOfExtraLives = 4;
static constexpr int pacmanMaxNumOfExtraLives = 5;
static constexpr int pacmanMaxFrameMove = 8;

/// --------------------------------------------
/// Ghosts
static constexpr int ghostMaxFrameMove = 2;

static constexpr int rightAboveCageCenterMapX =
    16;  // actually this center is between two cells (right cell is indicated)
static constexpr int leftAboveCageCenterMapX = rightAboveCageCenterMapX - 1;
static constexpr int aboveCageCenterMapY = 11;
static constexpr float aboveCageCenterPixX =
    (rightAboveCageCenterMapX + mapOffsetCellX_i) * cellSize_i;
static constexpr float aboveCageCenterPixY =
    (aboveCageCenterMapY + mapOffsetCellY_i + 0.5f) * cellSize_i;

static constexpr int cageCenterMapX =
    rightAboveCageCenterMapX;  // actually this center is between two cells
                               // (right cell is indicated)
static constexpr int cageCenterMapY = 14;
static constexpr float cageCenterPixX = aboveCageCenterPixX;
static constexpr float cageCenterPixY =
    (cageCenterMapY + mapOffsetCellY_i + 0.5f) * cellSize_i;

static constexpr int cageLeftCellMapX =
    cageCenterMapX -
    2;  // actually this position is between two cells (right cell is indicated)
static constexpr int cageLeftCellMapY = cageCenterMapY;
static constexpr float cageLeftCellPixX =
    (cageLeftCellMapX + mapOffsetCellX_i) * cellSize_i;
static constexpr float cageLeftCellPixY = cageCenterPixY;

static constexpr int cageRightCellMapX =
    cageCenterMapX +
    2;  // actually this position is between two cells (right cell is indicated)
static constexpr int cageRightCellMapY = cageCenterMapY;
static constexpr float cageRightCellPixX =
    (cageRightCellMapX + mapOffsetCellX_i) * cellSize_i;
static constexpr float cageRightCellPixY = cageCenterPixY;

static constexpr float cageMaxCenterPixY = cageCenterPixY + cellSize_f * 0.5f;
static constexpr float cageMinCenterPixY = cageCenterPixY - cellSize_f * 0.5f;

static constexpr int ghostEyesRectLeft = 0;
static constexpr int ghostEyesRectTop = entitySize_i * 4;

static constexpr int frightGhostRectLeft = entitySize_i * 4;
static constexpr int frightGhostRectTop = entitySize_i * 4;

static constexpr float maxTimeFright = 6000000.0f;
static constexpr float timeChangeFrightColor = mcsInSec / 5.0f;
static constexpr float timeStartChangeFrightColor =
    timeChangeFrightColor * 9.0f;

/// --------------------------------------------
/// Bonuses

/// --------------------------------------------
/// Scenes
extern bool event_MainMenu;
extern bool event_Close;
extern bool event_FullClose;
extern bool event_ChooseMode;
extern bool event_ClassicGame;
extern bool event_EndlessMap;
extern bool event_StartGame;
extern bool event_Pause;
extern bool event_MainMenuSettings;
extern bool event_PauseSettings;
extern bool event_BackMainMenu;
extern bool event_GameOver;
extern bool event_NewGame;
extern bool event_Profiles;
extern bool event_InDevelopment;

extern bool calledFromGameMode;
extern bool calledFromPause;
extern bool calledByExitButton;

static constexpr float timePauseBeforeKillAnimation = 0.75f * mcsInSec;
static constexpr float timePauseAfterKillAnimation = 2.0f * mcsInSec;
static constexpr float timePauseBeforeEndLevelAnimation = 1.0f * mcsInSec;
static constexpr float timeGameOverAnimation = 2.0f * mcsInSec;
static constexpr float timeEndLevelAnimation = 3.0f * mcsInSec;
static constexpr float timeChangeMapColor = 0.5f * mcsInSec;

template <class T>
T abs(T elem);

bool empty(std::fstream& flow);