#include "Observer.h"

Observer::Observer() : scenes_m(music_m) {
  //    readData();
}

void Observer::game() {
  while (!event_FullClose) {
    if (event_MainMenu)
      scenes_m.mainMenu.open(window_m, prevScene_m, techInfo_m);
    else if (event_Close)
      scenes_m.close.open(window_m, prevScene_m, techInfo_m);
    else if (event_ChooseMode)
      scenes_m.chooseMode.open(window_m, prevScene_m,
                               techInfo_m);  // todo change to switch case
    else if (event_ClassicGame)
      scenes_m.classicGame.open(window_m, prevScene_m, techInfo_m);
    else if (event_Pause)
      scenes_m.pause.open(window_m, prevScene_m, techInfo_m);
    else if (event_BackMainMenu)
      scenes_m.backMainMenu.open(window_m, prevScene_m, techInfo_m);
    else if (event_GameOver)
      scenes_m.gameOver.open(window_m, prevScene_m, techInfo_m);
    else if (event_MainMenuSettings)
      scenes_m.mainMenuSettings.open(window_m, prevScene_m, techInfo_m);
  }
  //    saveData();
}

// void Observer::readData() {
//
// }

// void Observer::saveData() {
//     scenes_m.classicGame.saveData();
// }