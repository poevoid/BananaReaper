#define ABG_IMPLEMENTATION
#define ABG_SYNC_PARK_ROW
#define SPRITESU_IMPLEMENTATION
#define OLED_SH1106  //for arduboy mini, set ABG_REFRESH_HZ TO 95
#define ABG_REFRESH_HZ 95
#define SPRITESU_OVERWRITE
#define SPRITESU_PLUSMASK
#define SPRITESU_FX
#include <ArduboyFX.h>
#include "src/ArduboyG.h"
#include "src/SpritesU.hpp"
#include "fxdata/fxdata.h"
ArduboyG_Config<ABG_Mode::L4_Triplane> arduboy;
// Include TileType
#include "func.h"


// Setup code
// Runs once at the start.
void setup()
{
	// Arduboy2 begin function
	// Runs the intro logo and provides other features.
	arduboy.boot();
  arduboy.startGray();
  FX::begin(FX_DATA_PAGE, FX_SAVE_PAGE);

	// Randomly generate the map.
	//generateMap();
}

// Game loop code
// Runs once per frame.
void loop()
{
	FX::enableOLED();

  arduboy.waitForNextPlane();
  //arduboy.invert(true);
  FX::disableOLED();
  if (arduboy.needsUpdate()) {
    arduboy.pollButtons();
    //handleInput();

	// Draw the tile map.
	 // drawTileMap();
    
    update();
  }
  render();
}