#pragma once
#include "Vector.h"

#define FRAME(x) x * 3 + arduboy.currentPlane()
#define MAX_FPS 10
#define WORLD_HEIGHT 3 //tiles
#define WORLD_WIDTH 13 //tiles
#define TILE_SIZE 64
#define SPLASH_TIME 75 //
#define NUM_TREES 12

bool doneAnimating = true;
int16_t cameraXminimum=0;
int16_t cameraXmaximum = 670;
int16_t cameraX=0;
int16_t cameraY;
int mapx = 0;
int mapy = -96;
int prevTime=0;
int startcounter=SPLASH_TIME;
uint8_t gamestate =0;

struct Text {
  int x, y;
  int counter;
};
struct World {
  int x;
  int y;
  uint24_t map;
  uint8_t framecount;
  uint8_t currentframe;

};
struct Sprite {
  uint8_t x, y;
  uint24_t sprite;
  uint8_t framecount;
  uint8_t currentframe;
  uint8_t framewait;
  int counter;
  bool inc;
};

struct Tree {
  uint8_t x, y;
  uint24_t sprite;
  uint8_t framecount;
  uint8_t currentframe;
  uint8_t framewait;
  int counter;
  bool inc;
  int ripeness;
  bool ripe;
};
struct Player {
  uint8_t x, y;
  uint24_t sprite;
  uint8_t framecount;
  uint8_t currentframe;
  uint8_t framewait;
  int counter;
  bool left;
  bool right;
  bool walking;
  bool reaping;
  bool inc;
};


enum class Screen : uint8_t {
  Splash,
  Title,
  Game,
  Gallery,
  Gameover
};
Tree tree[12] = {{0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false},
                  {0,0,bananatree,7, 0, 2, true, 0, false}};
Player grim = {32, 0, deathidleright,7,0,2,false,true,false,false,true};
Sprite sprite = {32,0,deathidleright,7, 0, 2, true};
Screen screen = {Screen::Splash};
Text text = {0,0,0};
