/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"
#include "view.h"

/* Joystick 5-way switch
*/
enum position { left,down,right,up,centre };
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};

bool controlPress (enum position p){
	return !joystick[p];
}
//Turn the ship to the right
void controlPressRight (void) {
	if (controlPress(right)) { 
		player.shipTrajectory = player.shipTrajectory - 20; 
	}
}
//Turn the ship to the left
void controlPressLeft (void) {
	if (controlPress(left)) { 
		player.shipTrajectory = player.shipTrajectory + 20; 
	}
}
//Accelerate the ship forward
void controlPressUp(void) {
	if (controlPress(up)) {
		player.speed = player.speed + 1;
	}
}
//Controls for the ship
void controls(void)
{
	controlPressLeft();
	controlPressRight();
	controlPressUp();
	//Press the down direction on the joystick to start from the main menu screen
	if(mainMenu== true && controlPress(down))
	{
		mainMenu = false;
	}
	//Restart the game after game over
	if(lives == 0 && controlPress(down)) {
		lives = 5;
		score = 0;
		shieldStatus = 3;
	}
	//Fire missiles when the centre button is pressed
	if (controlPress(centre) && lives > 0) {
		fireMissile();
	}
}

