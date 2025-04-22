#define ABG_IMPLEMENTATION
#define ABG_SYNC_PARK_ROW
#define SPRITESU_IMPLEMENTATION
//#define OLED_SH1106  //for arduboy mini, set ABG_REFRESH_HZ TO 95
//#define ABG_REFRESH_HZ 95
#define SPRITESU_OVERWRITE
#define SPRITESU_PLUSMASK
#define SPRITESU_FX
#include <ArduboyFX.h>
#include <EEPROM.h>
#include "src/ArduboyG.h"
#include "src/SpritesU.hpp"
#include "fxdata/fxdata.h"
ArduboyG_Config<ABG_Mode::L4_Triplane> arduboy;
// Include TileType

//BeepPin1 beep;
#include "func.h"


// Setup code
// Runs once at the start.
void setup() {
  // Arduboy2 begin function
  // Runs the intro logo and provides other features.
  arduboy.boot();
  arduboy.startGray();
  FX::begin(FX_DATA_PAGE, FX_SAVE_PAGE);
  FX::setFont(arduboyFont, dcmNormal);  // Initialize font here
  FX::setCursor(0, 0);
  //beep.begin();
  // Init default highscores if empty
  // Check if EEPROM at EE_START is uninitialized
  if (EEPROM.read(EE_START) == 0xFF) {
    HighScoreData defaults;
    // Initialize default highscores with all level scores
    memcpy(defaults.entries[0].initials, "Ban", 3);
    defaults.entries[0].lv1 = 5000;
    defaults.entries[0].lv2 = 0;
    defaults.entries[0].lv3 = 0;

    memcpy(defaults.entries[1].initials, "ana", 3);
    defaults.entries[1].lv1 = 4000;
    defaults.entries[1].lv2 = 0;
    defaults.entries[1].lv3 = 0;

    memcpy(defaults.entries[2].initials, "Jam", 3);
    defaults.entries[2].lv1 = 3000;
    defaults.entries[2].lv2 = 0;
    defaults.entries[2].lv3 = 0;

    memcpy(defaults.entries[3].initials, "Apr", 3); // New
    defaults.entries[3].lv1 = 2000;
    memcpy(defaults.entries[4].initials, "'25", 3); // New
    defaults.entries[4].lv1 = 1000;

    EEPROM.put(EE_START, defaults);
  }

  loadHighscores();

  //grim.x = 64;  // Start centered horizontally
  //grim.y = 32;  // Start at proper height
  //cameraX = grim.x - 64; // Center camera on player
  // Randomly generate the map.
  //generateMap();
  //while (!Serial) { ; }
  //Serial.begin(115200);  //for debug
  //delay(50);
  //Serial.println("debugging:");
}

// Game loop code
// Runs once per frame.
void loop() {

  arduboy.waitForNextPlane();
  //arduboy.invert(true);
  FX::disableOLED();
  if (arduboy.needsUpdate()) {
    //beep.timer();
    arduboy.pollButtons();
    update();
  }

  render();
  FX::enableOLED();
  //FX::disableOLED();
}