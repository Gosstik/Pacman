cmake_minimum_required(VERSION 3.22)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(./Images)
include_directories(./src)
include_directories(./src/Bonuses)
include_directories(src/Button)
include_directories(./src/Entity)
include_directories(./src/Map)
include_directories(./src/Scenes)
include_directories(./src/MyObjects/MyConvexShape)
include_directories(src/MyObjects/MySprite)
include_directories(src/MyObjects/MyText)
include_directories(src/MyObjects/MyWindow)
include_directories(./src/Observer)
include_directories(./src/additional)
include_directories(./src/Scenes/PopUpWindows)
include_directories(./src/Scenes/StaticScenes)
include_directories(./src/Score)
include_directories(./src/Scenes/StaticScenes/ClassicGame)
include_directories(./src/Entity/Ghosts)
include_directories(./src/Scenes/StaticScenes/ClassicGame/Intermissions)
include_directories(./src/TechInfo)
include_directories(./src/Scenes/Settings)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(source
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/additional/Globals.cpp
        ${SRC_DIR}/additional/Globals.h
        ${SRC_DIR}/MyObjects/MyWindow/MyWindow.cpp
        ${SRC_DIR}/MyObjects/MyWindow/MyWindow.h
        ${SRC_DIR}/Map/Map.cpp src/Map/Map.h
        ${SRC_DIR}/Entity/Entity.cpp
        ${SRC_DIR}/Entity/Entity.h
        ${SRC_DIR}/Entity/Pacman.cpp
        ${SRC_DIR}/Entity/Pacman.h
        ${SRC_DIR}/Entity/Ghosts/Ghosts.cpp
        ${SRC_DIR}/Entity/Ghosts/Ghosts.h
        ${SRC_DIR}/Entity/Ghosts/Ghost.cpp
        ${SRC_DIR}/Entity/Ghosts/Ghost.h
        ${SRC_DIR}/Scenes/Scenes.h
        ${SRC_DIR}/MyObjects/MyText/MyText.cpp
        ${SRC_DIR}/MyObjects/MyText/MyText.h
        ${SRC_DIR}/Bonuses/Bonuses.cpp
        ${SRC_DIR}/Bonuses/Bonuses.h
        ${SRC_DIR}/Button/Button.cpp
        ${SRC_DIR}/Button/Button.h
        ${SRC_DIR}/Map/Walls.cpp
        ${SRC_DIR}/Map/Walls.h
        ${SRC_DIR}/MyObjects/MySprite/Sprite1H.cpp
        ${SRC_DIR}/MyObjects/MySprite/Sprite1H.h
        ${SRC_DIR}/Scenes/StaticScenes/MainMenu.cpp
        ${SRC_DIR}/Scenes/StaticScenes/MainMenu.h
        ${SRC_DIR}/Observer/Observer.cpp
        ${SRC_DIR}/Observer/Observer.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/ClassicGame.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/ClassicGame.h
        ${SRC_DIR}/Scenes/StaticScenes/ChooseMode.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ChooseMode.h
        ${SRC_DIR}/TechInfo/FPS.cpp
        ${SRC_DIR}/TechInfo/FPS.h
        ${SRC_DIR}/Scenes/PopUpWindows/Close.cpp
        ${SRC_DIR}/Scenes/PopUpWindows/Close.h
        ${SRC_DIR}/Scenes/PopUpWindows/Pause.cpp
        ${SRC_DIR}/Scenes/PopUpWindows/Pause.h
        ${SRC_DIR}/Scenes/PopUpWindows/BackMainMenu.cpp
        ${SRC_DIR}/Scenes/PopUpWindows/BackMainMenu.h
        ${SRC_DIR}/Scenes/PopUpWindows/GameOver.cpp
        ${SRC_DIR}/Scenes/PopUpWindows/GameOver.h
        ${SRC_DIR}/Bonuses/Gums.cpp
        ${SRC_DIR}/Bonuses/Gums.h
        ${SRC_DIR}/Bonuses/BigGums.cpp
        ${SRC_DIR}/Bonuses/BigGums.h
        ${SRC_DIR}/Bonuses/Fruits.cpp
        ${SRC_DIR}/Bonuses/Fruits.h
        ${SRC_DIR}/Score/Score.cpp
        ${SRC_DIR}/Score/Score.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/ClassicGameLabels.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/ClassicGameLabels.h
        #        ${SRC_DIR}/additional/PathFinder.cpp ${SRC_DIR}/additional/PathFinder.h
        ${SRC_DIR}/Map/Portals.cpp
        ${SRC_DIR}/Map/Portals.h
        ${SRC_DIR}/Scenes/Settings/MainMenuSettings.cpp
        ${SRC_DIR}/Scenes/Settings/MainMenuSettings.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/ClassicGameMusic.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/ClassicGameMusic.h
        ${SRC_DIR}/additional/Music.cpp
        ${SRC_DIR}/additional/Music.h
        ${SRC_DIR}/Entity/Ghosts/Blinky.cpp
        ${SRC_DIR}/Entity/Ghosts/Blinky.h
        ${SRC_DIR}/Entity/Ghosts/Pinky.cpp
        ${SRC_DIR}/Entity/Ghosts/Pinky.h
        ${SRC_DIR}/Entity/Ghosts/Inky.cpp
        ${SRC_DIR}/Entity/Ghosts/Inky.h
        ${SRC_DIR}/Entity/Ghosts/Clyde.cpp
        ${SRC_DIR}/Entity/Ghosts/Clyde.h
        ${SRC_DIR}/MyObjects/MySprite/Sprite2H.cpp
        ${SRC_DIR}/MyObjects/MySprite/Sprite2H.h
        #        src/additional/Profiler.cpp src/additional/Profiler.h
        ${SRC_DIR}/TechInfo/Cycles.cpp
        ${SRC_DIR}/TechInfo/Cycles.h
        ${SRC_DIR}/TechInfo/TechInfo.cpp
        ${SRC_DIR}/TechInfo/TechInfo.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Intermissions/Intermission1.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Intermissions/Intermission1.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Intermissions/Intermissions.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Intermissions/Intermission2.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Intermissions/Intermission2.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Intermissions/Intermission3.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Intermissions/Intermission3.h
        ${SRC_DIR}/additional/ComingSoon.h
        ${SRC_DIR}/Button/PillButton.cpp
        ${SRC_DIR}/Button/PillButton.h
        ${SRC_DIR}/MyObjects/MyConvexShape/MyConvexShape.cpp
        ${SRC_DIR}/MyObjects/MyConvexShape/MyConvexShape.h
        ${SRC_DIR}/MyObjects/MyConvexShape/MyConvexShapeHelper.cpp
        ${SRC_DIR}/MyObjects/MyConvexShape/MyConvexShapeHelper.h
        ${SRC_DIR}/additional/PacmanTitle.h
        ${SRC_DIR}/Scenes/Settings/SettingsHelper.cpp
        ${SRC_DIR}/Scenes/Settings/SettingsHelper.h
        ${SRC_DIR}/Scenes/Settings/GameOptions.cpp
        ${SRC_DIR}/Scenes/Settings/GameOptions.h
        ${SRC_DIR}/Scenes/Settings/MusicOptions.cpp
        ${SRC_DIR}/Scenes/Settings/MusicOptions.h
        ${SRC_DIR}/Scenes/Settings/GraphicsOptions.cpp
        ${SRC_DIR}/Scenes/Settings/GraphicsOptions.h
        ${SRC_DIR}/Scenes/Settings/DevOptions.cpp
        ${SRC_DIR}/Scenes/Settings/DevOptions.h
        ${SRC_DIR}/Button/SettingHeaderButton.cpp
        ${SRC_DIR}/Button/SettingHeaderButton.h
        ${SRC_DIR}/Scenes/Settings/MusicOptionsHelper.cpp
        ${SRC_DIR}/Scenes/Settings/MusicOptionsHelper.h
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Preview.cpp
        ${SRC_DIR}/Scenes/StaticScenes/ClassicGame/Preview.h
        ${SRC_DIR}/additional/FileInfo.h
        ${SRC_DIR}/additional/FirstOpen.h
        ${SRC_DIR}/additional/PathStrings.cpp
        )

add_executable(Pacman ${source})

target_link_libraries(Pacman -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system)
