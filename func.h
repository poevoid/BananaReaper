#include "Arduino.h"
#include "Arduboy2Core.h"
#include "vars.h"
/*void drawWorld() {
  for (int y = 0; y < WORLD_HEIGHT; y++) {
    for (int x = 0; x < WORLD_WIDTH; x++) {
      SpritesU::drawPlusMaskFX(x * TILE_SIZE + mapx, y * TILE_SIZE + mapy, world.map, FRAME(worldarray[y][x]));
    }
  }
}
*/

void handleRipeness(){
  for (int i = 0; i < NUM_TREES; i++) {
    ++tree[i].ripeness;
    if (tree[i].ripeness >= 2000){
      tree[i].ripe = true;
    } else {
      tree[i].ripe = false;
    }

  }
}
void updateTrees(){
   for (int i = 0; i < NUM_TREES; i++) {
    //if (tree[i].ripeness > 333 & tree[i].ripeness < 666)
  }
}
void instantiateTrees() {
  //Tree tree[7] = {64,0,bananatree,7, 0, 2, true, 0, false};
  for (int i = 0; i < NUM_TREES; i++) {
    int newx = (i * 64);
    tree[i].x = newx;
    int newripeness = random(0,250);
    tree[i].ripeness = newripeness;
  }
}
void handleCamera() {
  if (arduboy.pressed(LEFT_BUTTON)) {
    grim.left = true;
    grim.right = false;
    if (cameraX > cameraXminimum)
      --cameraX;
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    grim.left = false;
    grim.right = true;
    ++cameraX;
  }
}
void drawTrees() {
  for (int i = 0; i < NUM_TREES; i++) {
    int16_t drawX = (i * 64) - cameraX;
    SpritesU::drawPlusMaskFX(drawX, tree[i].y, tree[i].sprite, FRAME(tree[i].currentframe));
  }
}

void displayTitle() {
  arduboy.setCursor(32, 32);
  arduboy.println("Banana Reaper");
}
void handleTitle() {
  if (arduboy.pressed(A_BUTTON)) {
    instantiateTrees();
    screen = Screen::Game;
  }
  if (arduboy.pressed(B_BUTTON)) {
    startcounter = SPLASH_TIME;
    screen = Screen::Splash;
  }
}
/*void drawIntro() {

  for (int y = 0; y < sprite.framecount; y++) {
    SpritesU::drawPlusMaskFX(text.x, y * 64 + text.y, sprite.sprite, FRAME(y));
  }

  if (text.counter % (FRAME(sprite.framewait)) == 0) {
    text.y--;
  }
  if (text.y <= -2769) {
    screen = Screen::Game;
  }
  text.counter += 1;
  if (arduboy.pressed(A_BUTTON)) {
    screen = Screen::Game;
  }
  delay(10);
}*/

template<typename T>
//This animaton function takes a sprite and runs through all its frames, then starts over at frame 0
void animateOnce(T& structObj, uint8_t T::*cframe, uint8_t T::*framec, int T::*counter, uint8_t T::*wait, uint24_t T::*sprite, bool T::*left, bool T::*right, bool T::*reaping) {
  structObj.*cframe = 0;
  if (structObj.*counter % (FRAME(structObj.*wait)) == 0) {
    if (structObj.*cframe < structObj.*framec) {
      structObj.*cframe += 1;
    } else {
      structObj.*reaping = false;
      
    }
  }
  structObj.*counter += 1;
}

template<typename T>
//This animaton function takes a sprite and runs through all its frames, then starts over at frame 0
void animateSprite(T& structObj, uint8_t T::*cframe, uint8_t T::*framec, int T::*counter, uint8_t T::*wait) {
  if (structObj.*counter % (FRAME(structObj.*wait)) == 0) {
    if (structObj.*cframe < structObj.*framec) {
      structObj.*cframe += 1;
    } else {
      structObj.*cframe = 0;
    }
  }
  structObj.*counter += 1;
}

void attack() {

   animateOnce(grim, &Player::currentframe, &Player::framecount, &Player::counter, &Player::framewait, &Player::sprite, &Player::left, &Player::right, &Player::reaping);
   
}

template<typename T>
//This animation function takes a sprite and runs trough all its frames in incremental order, then when reaching the final frame, in decremental order back to zero, ad infinitum
void animateFWB(T& structObj, uint8_t T::*cframe, uint8_t T::*framec, int T::*counter, uint8_t T::*wait, bool T::*inc) {
  if (structObj.*counter % (FRAME(structObj.*wait)) == 0) {
    if (structObj.*cframe == structObj.*framec) {
      structObj.*inc = false;
    }
    if (structObj.*cframe == 0) {
      structObj.*inc = true;
    }
    if (structObj.*inc) {
      if (structObj.*cframe < structObj.*framec) {
        structObj.*cframe += 1;
      }
    } else {
      if (structObj.*cframe > 0) {
        structObj.*cframe -= 1;
      }
    }
  }
  structObj.*counter += 1;
}

void playerInput() {
  handleCamera();
  if (arduboy.justPressed(A_BUTTON) && !grim.reaping) {
    grim.currentframe = 0;
    grim.reaping = true;
    
  } 
}


void updatePlayerSprite() {
  if (grim.left && !grim.reaping){grim.sprite=deathidleleft;}
  if (grim.right && !grim.reaping){grim.sprite=deathidleright;}
  if (grim.left && grim.reaping){grim.sprite=deathattackleft;}
  if (grim.right && grim.reaping){grim.sprite=deathattackright;} 
  if (grim.reaping && grim.currentframe ==grim.framecount){grim.reaping=false;}
}


void update() {
  switch (screen) {
    case Screen::Splash:
      if (startcounter != 0) {
        startcounter--;
      } else {
        screen = Screen::Title;
      }
      break;
    case Screen::Title:

      handleTitle();





      break;

    case Screen::Game:
      if (!arduboy.pressed(B_BUTTON)) {
      }
      //instantiateTrees();
      animateSprite(grim, &Player::currentframe, &Player::framecount, &Player::counter, &Player::framewait);
      handleCamera();
      updatePlayerSprite();
      playerInput();


      break;


    case Screen::Gallery:

      break;


    case Screen::Gameover:

      if (arduboy.justPressed(A_BUTTON)) {
        screen = Screen::Title;
      }
      if (arduboy.justPressed(B_BUTTON)) {
        screen = Screen::Game;
      }
      break;
  }
}


void render() {
  uint16_t currentPlane = arduboy.currentPlane();

  switch (screen) {

    case Screen::Splash:
      SpritesU::drawPlusMaskFX(0, 0, punklogooutlined, FRAME(0));
      break;

    case Screen::Title:

      //drawIntro();




      if (currentPlane <= 0) {  //dark gray
      }

      if (currentPlane <= 1) {  //gray
        //arduboy.setCursor(0, 0);
        //arduboy.println(mapy);
      }

      if (currentPlane <= 2) {  //white
        displayTitle();
      }
      break;

    case Screen::Game:
      //arduboy.fillScreen(WHITE);
      drawTrees();
      //grim.sprite = deathidleright;
      grim.x = 0;
      grim.y = 0;
      grim.framecount = 7;
      SpritesU::drawPlusMaskFX(grim.x, grim.y, grim.sprite, FRAME(grim.currentframe));


      if (currentPlane <= 0) {  //dark gray
      }

      if (currentPlane <= 1) {  //gray
      }

      if (currentPlane <= 2) {  //white
        arduboy.setCursor(0, 0);
        arduboy.println(cameraX);
        // arduboy.println(mapy);
      }

      break;


    case Screen::Gallery:

      break;

    case Screen::Gameover:

      break;
  }
}