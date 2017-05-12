/*
	Asteroids assignment - Small Embedded Systems
  By Keith Leighton 
	15025361 
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

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
struct ship player;

float Dt = 0.01f;

Ticker model, view, controller;

bool mainMenu = true;
bool paused = true;

int main()
{
		//Initialises the double buffer
    init_DBuffer();
    
	  //Attaches timers to the different files
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.05);
    
    lives = 5;
    
    paused = true;
    
}
