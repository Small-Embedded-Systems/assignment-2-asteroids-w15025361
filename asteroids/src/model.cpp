/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

//Sets the heap size for missiles and asteroids - the max on screen at any one time
static const int heapSizeMissile = 10;
static const int heapSizeAsteroid = 6;

static missile_t missileHeap[heapSizeMissile];
static asteroid_t asteroidHeap[heapSizeAsteroid];
static missile_t *missileFreeNodes;
static asteroid_t *asteroidFreeNodes;

struct missile *mActive = NULL;
struct asteroid *aActive = NULL;

float headingTrajectory;
float pointX;
float pointY;
float xleft;
float yleft;
float xright;
float yright;
int asteroidSize;
int shieldStatus = 3;

/* Nodes */
void missileFreeNode(missile_t *i) 
{
	i->next = missileFreeNodes;
	missileFreeNodes = i;
}

void asteroidFreeNode(asteroid_t *i) 
{
	i->next = asteroidFreeNodes;
	asteroidFreeNodes = i;
}

void initialiseMissileHeap(void) 
{ //Initialises the missile heap
	int i;
	for (i=0; i<(heapSizeMissile-1); i++) {
		missileHeap[i].next = &missileHeap[i+1];
	}
	missileHeap[i].next = NULL;
	missileFreeNodes = &missileHeap[0];
}

void initialiseAsteroidHeap(void) 
{ //Initialises the asteroid heap
	int i;
	for (i=0; i<(heapSizeAsteroid-1); i++) {
		asteroidHeap[i].next = &asteroidHeap[i+1];
	}
	asteroidHeap[i].next = NULL;
	asteroidFreeNodes = &asteroidHeap[0];
}

missile_t *allocateNodeMissile(void) 
{ //Allocates a node to a missile
	missile_t *missileNode = NULL;
	if (missileFreeNodes) {
		missileNode = missileFreeNodes;
		missileFreeNodes = missileFreeNodes ->next;
	}
	return missileNode;
}
asteroid_t *allocateNodeAsteroid(void) 
{ //Allocates a node to an asteroid
	asteroid_t *asteroidNode = NULL;
	if (asteroidFreeNodes) {
		asteroidNode = asteroidFreeNodes;
		asteroidFreeNodes = asteroidFreeNodes ->next;
	}
	return asteroidNode;
}


void originMissile(struct missile *ignited) 
{ //Modelling the missile
	int missileSpeed = 250;
	ignited->missileX = player.originX;
	ignited->missileY = player.originY;
	ignited->missileVelocityX = missileSpeed*(cos(player.headingTrajectory));
	ignited->missileVelocityY = missileSpeed*(sin(player.headingTrajectory));
	ignited->missileLife = 2;
	
}
void fireMissile(void) 
{ //Firing a missile
	struct missile *fireMissile = allocateNodeMissile();
	if(fireMissile) {
		fireMissile->next=mActive;
		mActive=fireMissile;
		originMissile(fireMissile);
	}
}
void missileState (struct missile*p) 
{ //Status of the missile; Destroy the missile after a collision
	for (; p ; p = p->next) {
		p->missileX += p->missileVelocityX * Dt;
		p->missileY += p->missileVelocityY * Dt;
		if(p->missileX < 0 ||460 < p->missileX) {
			p->missileLife = 0;
		}
		if(p->missileY < 10 || 260 < p->missileY) {
			p->missileLife = 0;
		}
		p->missileLife -= Dt;
		
		if(p->next->missileLife<=0) {
			struct missile *destroyed = p->next;
			p->next = p->next->next;
			missileFreeNode(destroyed);
		}
	}
}
void missileAsteroidCollision(struct asteroid *a, struct missile *m) 
{ //Checking for collisions between missiles and asteroids and destroying both when a collision occurs
	for (; m ; m = m->next) {
		for (; a ; a = a->next) {
			if (((m->missileX) > (a->asteroidX -25) && (m->missileX) < (a->asteroidX + 25)) && 
			((m->missileY) > (a->asteroidY - 25) && (m->missileY) < (a->asteroidY + 25))) {
				m->missileLife = 0;
				a->asteroidLife = 0;
			}
	  }
  }
}
void originAsteroid(struct asteroid * ignited) 
{ //Modelling the asteroid
	int asteroidSpeed = randrange(30,50); //Random asteroid speed to create more of a challenge
	int shipRadius = 70;
	asteroidSize = 25;
	ignited->asteroidX = randrange(0,460); //Random asteroid direction
	ignited->asteroidY = randrange(0,240);
	//prevents an asteroid spawning next to the ship
	if (((ignited->asteroidX) > (player.originX - shipRadius) && (ignited->asteroidX) < (player.originX + shipRadius)) && 
		((ignited->asteroidY) > (player.originY -shipRadius) && (ignited->asteroidX) < (player.originY + shipRadius))) 
	{
			ignited->asteroidX = randrange(20,460);
			ignited->asteroidY = randrange(20,240);
	} 
	ignited->asteroidVelocityX = randrange(-asteroidSpeed,asteroidSpeed); //Random asteroid speed
	ignited->asteroidVelocityY = randrange(-asteroidSpeed,asteroidSpeed);
	ignited->asteroidLife = randrange(900,1100);
}
void spawnAsteroid(void) 
{//Spawns asteroids in-game
	struct asteroid *spawn = allocateNodeAsteroid();
	
	if(spawn) {
		spawn->next = aActive;
		aActive = spawn;
		originAsteroid(spawn);
	}
}
void asteroidState (struct asteroid *p) 
{ //Asteroid status; destroy the asteroid after a collsion
	for(; p ; p = p->next) {
		p->asteroidX += p-> asteroidVelocityX * Dt;
		p->asteroidY += p-> asteroidVelocityY * Dt;
			
		if(p->next->asteroidLife<=0) { 
				struct asteroid *destroyed = p->next;
				p->next = p->next->next;
				asteroidFreeNode(destroyed);
			}
	}
}
void shipAsteroidCollision(struct asteroid *a) 
{ //Checking for collisions between the ship and asteroids
	int shipRadius = 40;
	for (; a ; a = a->next) {
		if(((a->asteroidX) > (player.originX - shipRadius) && (a->asteroidX) < (player.originX + shipRadius)) && 
			((a->asteroidY) > (player.originY - shipRadius) && (a->asteroidY) < (player.originY + shipRadius))) {
				a->asteroidLife = 0;
				if( shieldStatus <= 3 && shieldStatus > 0) {
					shieldStatus = shieldStatus - 1;
					player.originX = 220;
					player.originY = 120;
					a->asteroidLife = 0; //A collision destroys an asteroid
				}
				if(lives > 0 && shieldStatus == 0) { //Starts taking away lives if the shields are down
					lives=lives-1;
					player.originX = 220;
					player.originY = 120;
					player.speed = 0;
					a->asteroidLife = 0;
				}
				if(lives == 0) {
					paused = true;
				}
			}
		}
}
void wrapScreen(struct asteroid *a) 
{ //Wrap the ship and asteroids if the go off screen
	if (player.originX > 485) {
		player.originX = 5;
  }
	else if (player.originX < -5) {
		player.originX = 480;
	}
	else if (player.originY < -5) {
		player.originY = 265;
	}
	else if (player.originY > 270) {
		player.originY = 5;
	}
	
	for(; a ; a = a->next) {
		if (a->asteroidX > 480) {
			a->asteroidX = 0;
	}
		else if (a->asteroidX < 0){
			a->asteroidX = 480;
		}
		else if (a->asteroidY > 260){
			a->asteroidY = 0;
		}
		else if (a->asteroidY < 0) {
			a->asteroidY = 260;
		}
	}
}
void shipPosition(void) 
{ //Sets the initial ship position
	if (paused == true) {
	player.originX = 220;
	player.originY = 120;
	paused = false;
	}
}
void shipDrag(void) 
{ //Brakes the ship if no longer accelerating and sets a max speed
	if(player.speed > 0 ) {
		player.speed = player.speed - 0.03;
	}
	if (player.speed < 0) {
		player.speed = 0;
	}
	if(player.speed >= 3) {
		player.speed = 3;
	
  }
}
void scoreIncrease(void) 
{ //Increases the score as time goes on
	if ((elapsed_time > score) && (paused == false)) {
		score++;
	}
}
void shipTurning(void) 
{ //Turning the ship left and right
	player.headingTrajectory = radians(player.shipTrajectory);
  player.originX = player.originX + player.speed * (cos(player.headingTrajectory));
	player.originY = player.originY + player.speed * (sin(player.headingTrajectory));

	player.pointX = 15 * (cos(player.headingTrajectory));
	player.pointY = 15 * (sin(player.headingTrajectory));

	player.xleft = (-10 * cos(player.headingTrajectory)) - (10 *(sin(player.headingTrajectory)));
	player.yleft = (-10* sin(player.headingTrajectory)) + (10 *(cos(player.headingTrajectory)));

	player.xright = (-10 *(cos(player.headingTrajectory))) - (-10 * (sin(player.headingTrajectory)));
	player.yright = (-10 *(sin(player.headingTrajectory))) + (-10 * (cos(player.headingTrajectory)));
}
void physics(void)
{ 
	if(paused == true && lives == 5) 
	{ //Initialises the heaps as the game starts
		initialiseAsteroidHeap();
		initialiseMissileHeap();
	}
	//The physics of the game; brings together the models and works with view to show the game on screen
	shipTurning();
	scoreIncrease();
	shipDrag();
	wrapScreen(aActive);
	shipPosition();
	missileAsteroidCollision(aActive, mActive);
	shipAsteroidCollision(aActive);
	asteroidState(aActive);
	missileState(mActive);
	elapsed_time += Dt;
	
}

