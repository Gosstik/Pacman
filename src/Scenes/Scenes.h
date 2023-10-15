#pragma once

#include "BackMainMenu.h"
#include "ChooseMode.h"
#include "ClassicGame.h"
#include "Close.h"
#include "GameOver.h"
#include "MainMenu.h"
#include "MainMenuSettings.h"
#include "Music.h"
#include "MyWindow.h"
#include "Pause.h"

struct Scenes {
  MainMenu mainMenu;
  ChooseMode chooseMode;
  ClassicGame classicGame;
  Close close;
  Pause pause;
  BackMainMenu backMainMenu;
  GameOver gameOver;
  MainMenuSettings mainMenuSettings;

  explicit Scenes(Music& music)
      : mainMenu(music),
        chooseMode(music),
        classicGame(music),
        close(music),
        pause(music),
        backMainMenu(music),
        gameOver(music),
        mainMenuSettings(music) {}
};