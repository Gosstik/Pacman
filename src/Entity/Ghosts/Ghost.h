#pragma once

#include <algorithm>
#include <queue>
#include <random>
#include <vector>

#include "ClassicGameMusic.h"
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"

class Blinky;

enum GhostsMode { chase, scatter, frightened, dead };

struct GhostsTimes {
  const std::vector<float> frightTimesVec{
      6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 5.0f, 2.0f, 2.0f, 1.0f,
      5.0f, 2.0f, 1.0f, 1.0f, 3.0f, 1.0f, 1.0f, 0.0f, 1.0f};  // in seconds
  const std::vector<int> lvlChangeModeTimes{1, 2, 5};
  const std::vector<std::vector<float>> constScatterTimes{
      {7000000.0f, 7000000.0f, 5000000.0f, 5000000.0f},
      {7000000.0f, 7000000.0f, 5000000.0f, 16667.0f},
      {5000000.0f, 5000000.0f, 5000000.0f, 16667.0f}};
  const std::vector<std::vector<float>> constChaseTimes{
      {0.0f, 20000000.0f, 20000000.0f, 20000000.0f},
      {0.0f, 20000000.0f, 20000000.0f, 25000000.0f},
      {0.0f, 20000000.0f, 20000000.0f, 30000000.0f}};
  float frightTime{};
  float constTimeNextRelease = {};
  size_t lvlModeTime = 0;
  size_t currWave = 0;
  float accumTimeMode = 0;
  float accumTimeBigGumEffect = 0;
  float accumTimeNextRelease = 0;
  void setDefault() {
    accumTimeMode = 0;
    accumTimeBigGumEffect = 0;
    accumTimeNextRelease = 0;
    currWave = 0;
  }
  void updateTimeConsts(int level) {
    // time change mode (here we need to recount only index "lvlModeTime")
    size_t size = lvlChangeModeTimes.size();
    for (size_t i = 0; i < size; ++i)
      if ((i == size - 1) || level < lvlChangeModeTimes[i + 1]) {
        lvlModeTime = i;
        break;
      }

    if (level > frightTimesVec.size())
      frightTime = 0.0f;
    else
      frightTime = frightTimesVec[level - 1] * mcsInSec;

    if (level < 5)
      constTimeNextRelease = 4 * mcsInSec;
    else
      constTimeNextRelease = 3 * mcsInSec;
  }
};

struct GhostsStates {
  bool bigGumEffect = false;
  void setDefault() { bigGumEffect = false; }
};

class Ghost : public Entity {
 private:
  enum FrightColor { Blue = 0, White };

  struct Times {
    float constSimpleNextFrame{};
    float constFrightNextFrame{};
    float accumChangeFrightColor = 0;
    void setDefault() { accumChangeFrightColor = 0; }
  };

  struct States {
    bool alive{};
    bool simpleMoveInCage{};
    bool moveToCage = false;
    bool moveToCageCentralCell = false;
    bool releasing = false;
    void setDefault() {
      moveToCage = false;
      moveToCageCentralCell = false;
      releasing = false;
    };
  };

  struct UtilityStates {
    bool startInCage{};            // defined in ghost constructor
    bool startMoveToCage = false;  // for moveToCage
    bool inPlace = true;           // for all cage states
    bool bigGumEffect = false;     // for alive
    bool reachedAboveCageCell = false;
    bool reachedAboveCagePos = false;
    bool startChangeFrightColor = false;
    bool previewMode = false;
    void setDefault() {
      startMoveToCage = false;
      inPlace = true;
      bigGumEffect = false;
      reachedAboveCageCell = false;
    };
  };

  struct Target {
    sf::Sprite spr;
    int mapX = 0;
    int mapY = 0;
    explicit Target(const sf::Texture& texture) {
      spr.setTexture(texture);
      spr.setTextureRect({0, 162, 16, 16});
    }
    void setPosition(int x, int y) {
      mapX = x;
      mapY = y;
      spr.setPosition((mapOffsetCellX_f + float(mapX)) * cellSize_f,
                      (mapOffsetCellY_f + float(mapY)) * cellSize_f);
    }
    void setDefault() {
      setPosition(-mapOffsetCellX_i - 1, -mapOffsetCellY_i - 1);
    }
  };

  static const std::vector<std::pair<int, int>> cellsForbadeUpDir;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 protected:
  static constexpr int previewStartTexRectL_m =
      entitySize_i * ghostMaxFrameMove * Dir::Left;

  Times tm_m;
  States st_m;
  UtilityStates ust_m;

  /// additional
  FrightColor frightColor_m = Blue;
  std::vector<Dir> vecDir_m;
  std::random_device seed_m;
  std::mt19937 gen_m;
  std::queue<Dir> queue_m;
  std::vector<int> vecDist_m;
  Target target_m;
  float accumDistMoveToCage_m = 0;
  float normalSpeed_m = 0;
  float tunnelSpeed_m = 0;
  float frightSpeed_m = 0;
  float cageSpeed_m = 0;
  float moveToCageSpeed_m = 0;

  /// change in case adding or deleting variables
  //    void mySwap(Ghost& other);

 public:
  bool justBeenKilled_m = false;

  Ghost(const sf::Texture& texture, int ghostStartPosMapX,
        int ghostStartPosMapY, float ghostStartPosPixX, float ghostStartPosPixY,
        int ghostStartTexRectL, int ghostStartTexRectT, int level);
  /// change in case adding or deleting variables
  //    Ghost(const Ghost& other);
  //    Ghost& operator=(const Ghost& other);
  //    Ghost(Ghost&& other) noexcept;
  //    Ghost& operator=(Ghost&& other) noexcept;
  ~Ghost() override = default;

  void update(MyWindow& window, Map& map, Pacman& pacman, Blinky& blinky,
              GhostsStates& gsSt, GhostsTimes& gsTm, ClassicGameMusic& CGMusic,
              GhostsMode& mode, std::vector<Ghost*>& cagePopulation,
              const Ghost* releasingGhost, float dTime);
  void kill(Map& map, std::vector<Ghost*>& cagePopulation);
  void releaseFromCage();
  void setToCageCenterCell();

  void setBigGumEffect(Map& map);
  void setScatterMode(Map& map, Pacman& pacman);
  void setChaseMode(Map& map, Pacman& pacman, Blinky& blinky);

  void simpleReverseDir();

  void updateSpeed(float normalSpeed, float tunnelSpeed, float frightSpeed);

  /// preview functions
  void previewSetStart(float posX, float posY);
  void previewUpdate(float dTime);
  void previewChangeBehaviour();
  void previewKill();
  void previewUpdateSpeed(float normalSpeed, float frightSpeed);

  bool canBeEaten() const;
  bool isAlive() const;
  bool isInPlace() const;
  bool isMovingToCage() const;
  bool underBigGumEffect() const;

  sf::IntRect getTextureRect(const Dir& dir);

  /// change in case adding or deleting variables
  virtual void reloadAfterPacmanDeath();
  virtual void reloadNextLevel(int level, float normalSpeed, float tunnelSpeed,
                               float frightSpeed);
  virtual void reloadNewGame(float normalSpeed, float tunnelSpeed,
                             float frightSpeed);

  void calculateTimeConstants();  // calculate time variables constant per level

 protected:
  void updateVecDir(Map& map);
  void choosePreferableDirection();
  int distToTarget(const Dir& dir);
  int distToCell(int x, int y);

  /// virtual functions
  virtual void scatterChooseTarget(Pacman& pacman) = 0;
  virtual void chaseChooseTarget(Pacman& pacman, Blinky& blinky) = 0;

  void updateDirection(Map& map, Pacman& pacman, Blinky& blinky,
                       const GhostsMode& mode);

  void getDestination(std::vector<Ghost*>& cagePopulation,
                      const Ghost* releasingGhost);
  void changeFrightColor();
  void removeBigGumEffect();
  void handleSpeed();

  void move(Map& map, Pacman& pacman, Blinky& blinky, const GhostsMode& mode,
            float dTime);
  void moveToCage(Map& map, Pacman& pacman, Blinky& blinky,
                  std::vector<Ghost*>& cagePopulation,
                  const Ghost* releasingGhost, float dTime);
  void simpleMoveInCage(float dTime);
  void moveToCageCentralCell(float dTime);
  void moveReleaseFromCage(float dTime);

  void animation(float dTime);
  void animationMoveToCage();
  void animationBigGumEffect(GhostsTimes& gsTm, float dTime);
};