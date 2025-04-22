#pragma once

#define FRAME(x) x * 3 + arduboy.currentPlane()
#define WORLD_HEIGHT 3 // tiles
#define WORLD_WIDTH 13 // tiles
#define TILE_SIZE 64
#define SPLASH_TIME 75
#define NUM_TREES 12
#define EE_FILE 13 //slot 5-40 usually safe
#define MAX_HIGHSCORES 5

#pragma pack(push, 1)
struct HighScore {
    char initials[3];

    uint16_t lv1;
    uint16_t lv2;
    uint16_t lv3;
    uint16_t total() const { return lv1 + lv2 + lv3; }
};

struct HighScoreData {
    HighScore entries[5];  // Top 3 entries with level scores
    
    HighScoreData() {
        entries[0] = {'B','A','N', 5000, 6000, 7000};
        entries[1] = {'A','N','A', 4000, 5000, 6000};
        entries[2] = {'J','A','M', 3000, 4000, 5000};
        entries[3] = {'A','P','R', 2000, 3000, 4000}; 
        entries[4] = {'Y','2','5', 1000, 2000, 3000}; 
    }
};
#pragma pack(pop)

struct PlaythroughData {
    uint16_t scores[3] = {0, 0, 0};
    uint16_t total() const { return scores[0] + scores[1] + scores[2]; }
};



constexpr uint16_t HS_SIZE = sizeof(HighScore) * MAX_HIGHSCORES; // 5*5=25 bytes
constexpr uint16_t EE_START = EEPROM_STORAGE_SPACE_START + (EE_FILE * HS_SIZE);
constexpr uint16_t FRAMES_PER_SECOND = 30;
constexpr uint16_t SECONDS_TO_FRAMES(uint8_t seconds) { return seconds * FRAMES_PER_SECOND; }
constexpr uint8_t FRAMES_TO_SECONDS(uint16_t frames) { return frames / FRAMES_PER_SECOND; }

// Gameplay constants
constexpr uint8_t HIT_DURATION = 30;
constexpr int8_t KNOCKBACK_AMOUNT = 4;
// Level configuration [ripeness speed, duration]
constexpr uint8_t levelConfig[3][2] = {
  {8, 90},  // Level 1
  {12, 90}, // Level 2
  {20, 60}  // Level 3
};

constexpr uint16_t LEVEL_DURATION_FRAMES[3] = {
  levelConfig[0][1] * FRAMES_PER_SECOND,
  levelConfig[1][1] * FRAMES_PER_SECOND,
  levelConfig[2][1] * FRAMES_PER_SECOND
};

extern PlaythroughData currentPlaythrough; // Declaration only
extern HighScoreData highScores; // Global instance declaration

uint8_t currentInitial = 0;
char playerInitials[3] = {'A', 'A', 'A'};
bool scoresSaved = false;


enum class GameState {
    LevelStart,
    GoStart,
    Playing,
    LevelEnd,
    Paused
};

enum class Screen : uint8_t {
  Splash,
  Title,
  Game,
  Gameover,
  EnterInitials,
  Highscores
};


// ======== Game State ========
uint16_t frameCounter = 0;
GameState gameState = GameState::LevelStart;
int score = 0;
uint8_t currentLevel = 1;
int16_t playerScreenX = 64 - 32;  // Center the 64px-wide sprite


uint32_t gameTimer;
Screen screen = Screen::Splash;
int startcounter = SPLASH_TIME;
int prevTime = 0;
uint16_t currentPlane;

// ======== Camera ========
int16_t cameraX = 0;

// ======== Entity Structs ========
struct Buttons {
  uint8_t x, y;
  uint24_t sprite;
  uint8_t framecount;
  uint8_t currentframe;
  uint8_t framewait;
  int counter;
};
struct Tree {
  int16_t x, y;
  uint24_t sprite;
  uint8_t currentframe;
  int ripeness;
  bool ripe;
  uint8_t growthStage;  // 0-5
  bool isAnimating;
};

struct Player {
  int16_t worldX;
  uint8_t y;
  uint24_t sprite;
  uint8_t framecount;
  uint8_t currentframe;
  uint8_t framewait;
  int counter;
  bool left;
  bool right;
  bool reaping;
  bool isHit;
  uint8_t hitTimer;
  uint8_t attackFrameCounter;  // Added back
};

// ======== Entity Instances ========

Buttons Abutton = {70,0, abutton, 3, 0, 6, 0};
Buttons Bbutton = {0,0, bbutton, 3, 0, 6, 0};
Tree tree[NUM_TREES] = {
  {0,0,bananatree,0,0,false,0,false}, // Repeat for all NUM_TREES
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  {0,0,bananatree,0,0,false,0,false},
  
};

Player grim = {
  /*worldX*/ WORLD_WIDTH * TILE_SIZE / 2,
  /*y*/ 0,
  deathidleright,  // Default sprite
  7,               // framecount
  0,               // currentframe
  2,               // framewait
  0,               // counter
  false,           // left
  true,            // right
  false,           // reaping
  false,           // isHit
  1,               // hitTimer
  0                // attackFrameCounter
};