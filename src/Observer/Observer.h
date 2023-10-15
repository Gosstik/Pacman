#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <stack>

#include "Globals.h"
#include "Music.h"
#include "MyText.h"
#include "MyWindow.h"
#include "Scenes.h"
#include "TechInfo.h"
#include "TechInfo/FPS.h"

class Observer {
  Music music_m;
  MyWindow window_m;
  FPS fps_m;
  TechInfo techInfo_m;
  Scenes scenes_m;
  std::stack<std::reference_wrapper<bool>> prevScene_m;
  sf::Event event_m{};

 public:
  Observer();
  void game();
  //    void readData();
  //    void saveData();
};