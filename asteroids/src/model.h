/* Asteroids Model */
struct point {
    float x,y;
};
typedef struct point coordinate_t;
typedef struct point vector_t;

//Missile structure
typedef struct missile {
	float missileX;
	float missileY;
	float missileVelocityX;
	float missileVelocityY;
	float missileList;
	float missileLife; //Lifetime
	struct missile *next;
} missile_t;

//Asteroid structure
typedef struct asteroid {
	float asteroidX;
	float asteroidY;
	float asteroidVelocityX;
	float asteroidVelocityY;
	float asteroidLife; //Lifetime
	float asteroidSize;
	struct asteroid *next;
} asteroid_t;

//Ship structure
struct ship {
  coordinate_t p;
  vector_t     v;
	float originX;
	float originY;
	float xleft;
	float yleft;
	float xright;
	float yright;
	float pointX;
	float pointY;
	float headingTrajectory;
	float shipTrajectory;
	float speed;
};

//Nodes
missile_t *allocateNodeMissile(void);
asteroid_t *allocateNodeAsteroid(void);

/* initial struts for building linked lists */
struct rock {
  coordinate_t p;
  struct rock *next;
};

/*Functions*/
void physics(void);
void asteroidFreeNode(asteroid_t *i);
void missileFreeNode(missile_t *i);
void initialiseAsteroidHeap(void);
void initialiseMissileHeap(void);
void fireMissile(void);
void spawnAsteroid(void);
void shipDrag(void);
void scoreIncrease(void);
void shipTurning(void);

//External variables
extern double shipSpeed;
extern struct missile *mActive;
extern struct asteroid *aActive;
extern const int heapSizeAsteroid;
extern const int heapSizeMissile;
extern int asteroidSize;
extern int shieldStatus;