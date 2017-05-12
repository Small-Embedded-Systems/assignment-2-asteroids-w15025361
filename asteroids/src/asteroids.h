/* Game state */

extern float elapsed_time; /* time this ship has been active */
extern int   score;        /* total score so far */
extern int   lives;        /* lives remaining */
extern bool paused;        /* whether the game is paused or not */
extern struct ship player; /* The structure of the ship */

extern struct rock *asteroids; /* array of rocks / pointer to linked-list */
extern struct missile *shots;  /* array of missiles / pointer to linked-list */

extern const float Dt; /* Time step for physics, needed for consistent motion */

extern bool mainMenu; /* Whether the game is at the start screen or not */

