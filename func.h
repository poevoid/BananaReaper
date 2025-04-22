#include <stdint.h>
#include "Arduboy2.h"
#include "Arduino.h"
#include "Arduboy2Core.h"
#include "vars.h"
PlaythroughData currentPlaythrough;
HighScoreData highScores;
/*void drawWorld() {
  for (int y = 0; y < WORLD_HEIGHT; y++) {
    for (int x = 0; x < WORLD_WIDTH; x++) {
      SpritesU::drawPlusMaskFX(x * TILE_SIZE + mapx, y * TILE_SIZE + mapy, world.map, FRAME(worldarray[y][x]));
    }
  }
}
*/

// For hit animation (controls 'isHit' flag)
template<typename T>
void animateOnceHit(T& structObj, uint8_t T::*cframe, uint8_t T::*framec,
                    int T::*counter, uint8_t T::*hitTimer, uint24_t T::*sprite,
                    bool T::*left, bool T::*right, bool T::*isHit, bool T::*reaping) {
  if (structObj.*counter % (FRAME(structObj.*hitTimer)) == 0) {
    if (structObj.*cframe < structObj.*framec) {
      structObj.*cframe += 1;
    } else {
      structObj.*isHit = false;  // Auto-reset flag
      structObj.*reaping = false;
      structObj.*cframe = 0;  // Reset animation
      if (grim.left == true) {
        structObj.*sprite = deathidleleft;
      } else {
        structObj.*sprite = deathidleright;
      }
    }
  }
  structObj.*counter += 1;
}

// For attack animation (controls 'reaping' flag)
template<typename T>
void animateOnceAttack(T& structObj, uint8_t T::*cframe, uint8_t T::*framec,
                       int T::*counter, uint8_t T::*wait, uint24_t T::*sprite,
                       bool T::*left, bool T::*right, bool T::*reaping) {
  if (structObj.*counter % (FRAME(structObj.*wait)) == 0) {
    if (structObj.*cframe < structObj.*framec) {
      structObj.*cframe += 1;
    } else {
      structObj.*reaping = false;  // Auto-reset flag
      structObj.*cframe = 0;       // Reset animation
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


void loadHighscores() {
  EEPROM.get(EE_START, highScores);
}

void saveHighscores() {
  EEPROM.put(EE_START, highScores);
}


void enterHighScore() {
  HighScore newEntry;
  memcpy(newEntry.initials, playerInitials, 3);
  newEntry.lv1 = currentPlaythrough.scores[0];
  newEntry.lv2 = currentPlaythrough.scores[1];
  newEntry.lv3 = currentPlaythrough.scores[2];

  // Insert the new entry into the high scores list
  for (byte i = 0; i < MAX_HIGHSCORES; i++) {
    if (newEntry.total() > highScores.entries[i].total()) {
      for (byte j = MAX_HIGHSCORES - 1; j > i; j--) {
        highScores.entries[j] = highScores.entries[j - 1];
      }
      highScores.entries[i] = newEntry;
      break;
    }
  }
  saveHighscores();
}


void displayInitials() {
  byte index = 0;
  //while (true) {
  //arduboy.clear();
  arduboy.setCursor(16, 0);
  arduboy.print("HIGH SCORE");
  arduboy.setCursor(88, 0);
  arduboy.print(score);

  // Draw initials
  arduboy.setCursor(56, 20);
  arduboy.print(playerInitials[0]);
  arduboy.print(" ");
  arduboy.print(playerInitials[1]);
  arduboy.print(" ");
  arduboy.print(playerInitials[2]);
  if (currentInitial == 0) {
    arduboy.setCursor(56, 29);
    arduboy.print("^");
  } else if (currentInitial == 1) {
    arduboy.setCursor(56 + 12, 29);
    arduboy.print("^");
  } else if (currentInitial == 2) {
    arduboy.setCursor(56 + 24, 29);
    arduboy.print("^");
  }


  //arduboy.display();
  //arduboy.delayShort(100);
  //}
}

void renderHighscores() {
  arduboy.setCursor(32, 0);
  arduboy.print("HIGH SCORES");
  for (byte i = 0; i < MAX_HIGHSCORES; i++) {
    arduboy.setCursor(16, 10 + i * 10);
    arduboy.print(highScores.entries[i].initials[0]);
    arduboy.print(highScores.entries[i].initials[1]);
    arduboy.print(highScores.entries[i].initials[2]);
    arduboy.print(" ");
    arduboy.print(highScores.entries[i].total());
  }
  arduboy.setCursor(68, 16);
  arduboy.print("YOUR SCORE:");
  arduboy.setCursor(84, 26);
  arduboy.print(currentPlaythrough.total());
}
void handleInitialsInput() {
  if (arduboy.justPressed(LEFT_BUTTON)) currentInitial = max(currentInitial - 1, 0);
  if (arduboy.justPressed(RIGHT_BUTTON)) currentInitial = min(currentInitial + 1, 2);
  if (arduboy.justPressed(UP_BUTTON)) playerInitials[currentInitial]++;
  if (arduboy.justPressed(DOWN_BUTTON)) playerInitials[currentInitial]--;
  if (arduboy.justPressed(A_BUTTON)) {
    enterHighScore();
    scoresSaved = true;
    screen = Screen::Highscores;
  }
}

void highscoresInput() {
  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) {
    screen = Screen::Title;
  }
}

void switchGameover() {
  if (!scoresSaved && currentPlaythrough.total() > highScores.entries[4].total()) {
    screen = Screen::EnterInitials;
  } else {
    screen = Screen::Highscores;
  }
}

void handleRipeness() {
  uint8_t ripeningSpeed = levelConfig[currentLevel - 1][0];
  for (int i = 0; i < NUM_TREES; i++) {
    tree[i].ripeness += ripeningSpeed;

    // Update growth stage (0-5)
    tree[i].growthStage = min(tree[i].ripeness / 2000, 5);

    // Set animation state
    tree[i].isAnimating = (tree[i].growthStage >= 3);
    tree[i].ripe = tree[i].isAnimating;
  }
}

void updateTrees() {
  for (int i = 0; i < NUM_TREES; i++) {
    if (tree[i].isAnimating) {
      // Animate between frames 4-6 (indexes 3-5)
      tree[i].currentframe = 3 + (millis() / 300 % 3);
    } else {
      // Show growth stage frame (0-3)
      tree[i].currentframe = min(tree[i].growthStage, 3);
    }
  }
}

void instantiateTrees() {
  //Tree tree[7] = {64,0,bananatree,7, 0, 2, true, 0, false};

  for (int i = 0; i < NUM_TREES; i++) {
    tree[i].x = (i * 96);  // Offset trees by half tile (32px)
    tree[i].y = 0;         // Ground level
    tree[i].ripeness = random(0, 10000);
  }
  gameTimer = levelConfig[0][1];  // Start with level 1 timer
  currentLevel = 1;
  score = 0;
  memset(&currentPlaythrough, 0, sizeof(currentPlaythrough));
  prevTime = millis();
}


void handleCamera() {
  // Pure camera calculation, no input handling
  // Smooth camera follow with knockback effect
  const int16_t screenCenter = 64;
  int16_t targetX = grim.worldX - screenCenter;

  // Add slight camera offset during hit
  //if (grim.isHit) {
  //targetX += grim.hitKnockback * 2;  // Amplify camera effect
  //}

  cameraX = constrain(targetX, 0, (WORLD_WIDTH * TILE_SIZE) - 128);
}
void drawTrees() {
  for (int i = 1; i < NUM_TREES; i++) {
    int16_t screenX = tree[i].x - cameraX;
    if (screenX >= -64 && screenX < 192) {  // Only draw visible trees
      SpritesU::drawPlusMaskFX(screenX, tree[i].y, tree[i].sprite, FRAME(tree[i].currentframe));
    }
  }
}
void resetGame() {
  instantiateTrees();                                          // Reset trees, level, timer, and score
  memset(&currentPlaythrough, 0, sizeof(currentPlaythrough));  // Clear playthrough data
  gameState = GameState::LevelStart;                           // Reset level progression
  frameCounter = 0;
  scoresSaved = false;

  // Reset player state
  grim.worldX = WORLD_WIDTH * TILE_SIZE / 2;
  grim.y = 0;
  grim.sprite = deathidleright;
  grim.currentframe = 0;
  grim.counter = 0;
  grim.left = false;
  grim.right = true;
  grim.reaping = false;
  grim.isHit = false;

  // Reset camera
  cameraX = grim.worldX - 64;  // Center camera on player
  prevTime = millis();         // Sync timing
}
void displayTitle() {
  //arduboy.setCursor(32, 32);
  //arduboy.println("Banana Reaper");

  SpritesU::drawPlusMaskFX(0, 0, titleskull, FRAME(0));
  SpritesU::drawPlusMaskFX(128-76, 32, titlecard, FRAME(0));

  SpritesU::drawPlusMaskFX(Abutton.x, Abutton.y, Abutton.sprite, FRAME(Abutton.currentframe));
  arduboy.setCursor(Abutton.x, Abutton.y+16);
  arduboy.print("Highscores");
  SpritesU::drawPlusMaskFX(Bbutton.x, Bbutton.y, Bbutton.sprite, FRAME(Bbutton.currentframe));
  arduboy.setCursor(Bbutton.x, Bbutton.y+16);
  arduboy.print("Start");
}
void handleTitle() {
  if (arduboy.justPressed(B_BUTTON)) {
    ////Serial.println("Starting new game");
    resetGame();
    screen = Screen::Game;  // DEBUG: Force immediate gameover
    // screen = Screen::Gameover; // Test direct transition
    scoresSaved = false;  // Reset highscore state
    prevTime = millis();
  }
  if (arduboy.justPressed(A_BUTTON)) {
    //startcounter = SPLASH_TIME;
    //screen = Screen::Splash;
    screen = Screen::Highscores;
  }

  
}


void playerInput() {
  if (grim.isHit) return;

  // Movement handling
  const int16_t moveSpeed = 2;
  const int16_t worldWidth = WORLD_WIDTH * TILE_SIZE;

  if (!grim.reaping) {  // Only allow movement when not attacking
    if (arduboy.pressed(LEFT_BUTTON)) {
      grim.worldX = max(grim.worldX - moveSpeed, 64);
      grim.left = true;
      grim.right = false;
    }
    if (arduboy.pressed(RIGHT_BUTTON)) {
      grim.worldX = min(grim.worldX + moveSpeed, worldWidth - 64);
      grim.left = false;
      grim.right = true;
    }
  }

  // Attack handling
  if (arduboy.justPressed(A_BUTTON) && !grim.reaping) {
    // ... attack initiation code ...
    grim.reaping = true;
    grim.currentframe = 0;
    grim.attackFrameCounter = 0;
    // Preserve attack direction
    // bool originalLeft = grim.left;
    // bool originalRight = grim.right;
    // Calculate attack boundaries
    const int16_t playerCenter = 64;
    const int16_t attackWidth = 45;

    // Determine attack zone based on direction
    int16_t attackStart, attackEnd;
    int16_t attackOrigin;

    if (grim.left) {
      attackStart = playerCenter - attackWidth;
      attackEnd = playerCenter;
      attackOrigin = playerCenter - attackWidth;
    } else {
      attackStart = playerCenter;
      attackEnd = playerCenter + attackWidth;
      attackOrigin = playerCenter;
    }

    // Find closest tree
    int16_t closestDistance = 9999;
    int16_t targetTree = -1;

    for (int i = 0; i < NUM_TREES; i++) {
      int16_t treeScreenX = tree[i].x - cameraX;
      int16_t treeCenterX = treeScreenX + 22;

      if (treeCenterX >= attackStart && treeCenterX <= attackEnd) {
        int16_t distance = abs(attackOrigin - treeCenterX);

        if (distance < closestDistance) {
          closestDistance = distance;
          targetTree = i;
        }
      }
    }

    if (targetTree != -1) {
      if (tree[targetTree].ripe) {
        score += 100;
        tree[targetTree].ripeness = 0;
      } else {
        score = max(0, score - 50);
        // grim.pendingHit = true;
        grim.isHit = true;
        // grim.hitTimer = HIT_DURATION;
        // Apply single knockback impulse to player position
        //grim.worldX = grim.worldX + (grim.left ? KNOCKBACK_AMOUNT : -KNOCKBACK_AMOUNT);
        // Constrain player position
        // grim.worldX = constrain(grim.worldX, 64, (WORLD_WIDTH * TILE_SIZE) - 64);
      }
    }
    // grim.left = originalLeft;
    // grim.right = originalRight;
  }
}

void updatePlayerSprite() {
  if (grim.isHit) {
    grim.sprite = grim.left ? deathhitleft : deathhitright;
    grim.worldX = grim.worldX + (grim.left ? KNOCKBACK_AMOUNT : -KNOCKBACK_AMOUNT);
    grim.framecount = 1;  //indexed at zero
    animateOnceHit(grim, &Player::currentframe, &Player::framecount,
                   &Player::counter, &Player::hitTimer, &Player::sprite,
                   &Player::left, &Player::right, &Player::isHit, &Player::reaping);

  } else if (grim.reaping) {
    // Show attack animation frames
    grim.framecount = 7;
    grim.sprite = grim.left ? deathattackleft : deathattackright;
    animateOnceAttack(grim, &Player::currentframe, &Player::framecount,
                      &Player::counter, &Player::framewait, &Player::sprite,
                      &Player::left, &Player::right, &Player::reaping);

  } else {
    // Idle state handling
    grim.framecount = 7;
    grim.sprite = grim.left ? deathidleleft : deathidleright;
    animateSprite(grim, &Player::currentframe, &Player::framecount,
                  &Player::counter, &Player::framewait);
  }
}

void handleLeveling() {
  switch (gameState) {
    case GameState::LevelStart:
      frameCounter++;
      if (frameCounter >= FRAMES_PER_SECOND) {  // Show "LEVEL X" for 1 second
        gameState = GameState::GoStart;
        frameCounter = 0;
      }
      break;

    case GameState::GoStart:
      frameCounter++;
      if (frameCounter >= FRAMES_PER_SECOND) {
        gameState = GameState::Playing;
        frameCounter = 0;
        // Reset gameplay elements and score
        grim.worldX = WORLD_WIDTH * TILE_SIZE / 2;
        for (int i = 0; i < NUM_TREES; i++) {
          tree[i].ripeness = random(0, 10000);
        }
        score = 0;  // Reset score for the new level
      }
      break;

    case GameState::Playing:
      /* if (arduboy.justPressed(B_BUTTON)) { //debug function to get to highscore entry
        currentLevel = 3;
        frameCounter = 0;
        gameState = GameState::LevelEnd;
      }
      */
      if (arduboy.justPressed(B_BUTTON)) {
        gameState = GameState::Paused;
      } else {
        frameCounter++;
        // Update game logic every frame
        handleRipeness();
        updateTrees();
        updatePlayerSprite();
        playerInput();
        handleCamera();

        // Check level completion
        if (frameCounter >= LEVEL_DURATION_FRAMES[currentLevel - 1]) {
          gameState = GameState::LevelEnd;
          frameCounter = 0;
        }
      }
      break;

    case GameState::LevelEnd:
      frameCounter++;
      if (frameCounter >= FRAMES_PER_SECOND * 2) {
        if (currentLevel == 3) {
          // DEBUG: Force immediate transition
          //FX::display(CLEAR_BUFFER);
          Serial.println("TRANSITIONING TO GAMEOVER");
          currentPlaythrough.scores[2] = score;

          Serial.print("Final score: ");
          Serial.println(score);
          //return;

          screen = Screen::Gameover;
          //FX::enableOLED();

        } else {
          currentPlaythrough.scores[currentLevel - 1] = score;
          currentLevel++;
          gameState = GameState::LevelStart;
          score = 0;
        }
        frameCounter = 0;
      }
      break;

    case GameState::Paused:
      if (arduboy.justPressed(B_BUTTON)) {
        gameState = GameState::Playing;  // Resume
      }
      break;
  }
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
      animateSprite(Abutton, &Buttons::currentframe, &Buttons::framecount, &Buttons::counter, &Buttons::framewait);
  animateSprite(Bbutton, &Buttons::currentframe, &Buttons::framecount, &Buttons::counter, &Buttons::framewait);
      break;

    case Screen::Game:
      handleLeveling();
      break;

    case Screen::Gameover:
      switchGameover();  // Modify this to transition to EnterInitials if needed
      break;

    case Screen::EnterInitials:
      handleInitialsInput();  // Handle input per frame

      break;

    case Screen::Highscores:
      // Allow player to go back to title
      highscoresInput();
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
      
      displayTitle();
      if (currentPlane <= 0) {  //dark gray
      }

      if (currentPlane <= 1) {  //gray
      }

      if (currentPlane <= 2) {  //white
       
      }
      break;

    case Screen::Game:
      drawTrees();
      //grim.sprite = deathidleright;

      SpritesU::drawPlusMaskFX(playerScreenX, grim.y, grim.sprite, FRAME(grim.currentframe));
      // arduboy.drawRect(attackStart, 0, attackEnd-attackStart, 64, WHITE);
      // Draw attack zones
      if (grim.left && grim.reaping) {
        //arduboy.drawRect(64 - 45, 0, 45, 64, WHITE);
      } else if (grim.reaping) {  ///debug functions to show hitboxes
        //arduboy.drawRect(64, 0, 45, 64, WHITE);
      }
      if (currentPlane <= 1) {  //dark gray
        if (gameState == GameState::LevelStart) {
          arduboy.setTextSize(2);
          arduboy.setCursor(32, 20);
          arduboy.print("LEVEL ");
          arduboy.print(currentLevel);
          arduboy.setTextSize(1);
        } else if (gameState == GameState::GoStart) {
          arduboy.setTextSize(2);
          arduboy.setCursor(48, 20);
          arduboy.print("GO!");
          arduboy.setTextSize(1);
        } else if (gameState == GameState::Paused) {
          arduboy.setTextSize(2);
          arduboy.setCursor(40, 20);
          arduboy.print("PAUSED");
          arduboy.setTextSize(1);
        }
      }

      if (currentPlane <= 1) {  //gray
        arduboy.setCursor(0, 0);
        arduboy.print("Lv:");
        arduboy.print(currentLevel);

        arduboy.setCursor(50, 0);
        arduboy.print("Time:");
        // Calculate remaining time
        uint16_t remainingFrames = LEVEL_DURATION_FRAMES[currentLevel - 1] - frameCounter;
        uint8_t remainingSeconds = FRAMES_TO_SECONDS(remainingFrames);

        // Add 1 to prevent showing 0 during final second
        arduboy.print(remainingSeconds + (remainingFrames % FRAMES_PER_SECOND ? 1 : 0));


        arduboy.setCursor(70, 45);
        arduboy.print("Score:");
        arduboy.print(score);
        // Transition text
      }

      if (currentPlane <= 2) {  //white
        // arduboy.setCursor(0, 0);
        // arduboy.println(cameraX);
        // arduboy.println(mapy);
      }
      break;

    case Screen::Gameover:
      //renderGameover();
      break;

    case Screen::EnterInitials:
      if (currentPlane <= 2) {  //2 = white 1= grey 0= dark gray
        displayInitials();
      }
      break;

    case Screen::Highscores:
      // Display high scores
      renderHighscores();
      break;
  }
}