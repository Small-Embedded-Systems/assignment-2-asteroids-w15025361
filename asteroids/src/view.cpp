/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

const colour_t background = rgb(0,0,0); /* black */

/* double buffering functions */
void init_DBuffer(void)
{ /* initialise the LCD driver to use second frame in buffer */
  uint16_t *bufferbase = graphics->getFb();
  uint16_t *nextbuffer = bufferbase+ (480*272);
  LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{ /* swaps frames used by the LCD driver and the graphics object */
  uint16_t *buffer = graphics->getFb();
  graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
  LPC_LCD->UPBASE = (uint32_t)buffer;
}

void drawArena(void)
{ //Draws the start screen
	graphics->setTextSize(3);
	graphics->setCursor(170,100);
	graphics->printf("Asteroids");
	graphics->setTextSize(2);
	graphics->setCursor(150,130);
	graphics->printf("By Keith Leighton");
	graphics->setCursor(70,200);
	graphics->setTextSize(3);
	graphics->printf("Press DOWN to start!");
}

void drawShip(void)
{ //Draws the ship
	graphics->drawLine(player.originX + player.xleft, player.originY + player.yleft, player.originX + player.pointX, player.originY + player.pointY, WHITE);
	graphics->drawLine(player.originX + player.xleft, player.originY + player.yleft, player.originX + player.xright, player.originY + player.yright, WHITE);
	graphics->drawLine(player.originX + player.xright, player.originY + player.yright, player.originX + player.pointX, player.originY + player.pointY, WHITE);
}

void drawShield(void) 
{ //Draws the shield at different strengths
	if (shieldStatus == 3) {
	graphics->drawCircle(player.originX, player.originY, 20, GREEN);
	}
	if (shieldStatus == 2) {
	graphics->drawCircle(player.originX, player.originY, 20, YELLOW);
	}
	if (shieldStatus == 1) {
	graphics->drawCircle(player.originX, player.originY, 20, RED);
	}
	if (shieldStatus == 0) {
	graphics->drawCircle(player.originX, player.originY, 20, BLACK);}
}

void drawLayout(void) 
{ //Draws the "lives left" on the screen in real time
	graphics->setCursor(0,5);
	graphics->setTextSize(1);
	graphics->printf("Lives left: %d", lives);
}

void drawAsteroids(struct asteroid *list) 
{ //Draws the asteroids
	while(list) {
		graphics->drawCircle(list->asteroidX, list->asteroidY, asteroidSize, WHITE);
		list=list->next;
	}
}

void drawMissiles(struct missile *list) 
{ //Draws the missiles as they spawn from the ship
	while(list) {
		graphics->fillCircle(list->missileX, list->missileY, 5, WHITE);
		list=list->next;
	}
}

void drawGameOver(void) 
{ //Draws the screen that appears when lives = 0
	graphics->setTextSize(3);
	graphics->setCursor(160,100);
	graphics->printf("GAME OVER");
	graphics->setTextSize(2);
	graphics->setCursor(180,130);
	graphics->printf("Score: %d", score);
	graphics->setCursor(110,160);
	graphics->printf("Press DOWN to restart!");
}

void drawScore(void) 
{ //Draws the score on screen in realtime
	graphics->setCursor(0,100);
	graphics->setTextSize(1);
	graphics->printf("Score: %d", score);
}
void draw(void)
{ //Main draw function
  graphics->fillScreen(background);
    
	if (mainMenu == true)
	{
		drawArena();

	}
	else if (mainMenu == false && lives > 0)
	{
		drawShip();
		drawLayout();
		drawAsteroids(asteroidActive);
		drawMissiles(missileActive);
		spawnAsteroid();
		drawScore();
		drawShield();
	}
	else if (lives == 0) {
		drawGameOver();
		elapsed_time = elapsed_time - elapsed_time;
		paused = true;
	}
	swap_DBuffer();
}
