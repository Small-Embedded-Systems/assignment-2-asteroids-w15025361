/* Asteroids View */

/* support double buffering */
void init_DBuffer(void);
void swap_DBuffer(void);

/* functions */
void draw(void);
void drawLayout(void);
void drawShield(void);
void drawAsteroids(struct asteroid *list);
void drawMissiles(struct missile *list);
void fireMissile(void);
