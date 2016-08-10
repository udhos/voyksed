
#ifndef SHIP_H
#define SHIP_H


#include "frame.h"
#include "sprite.h"


struct ship_t {
  const char *name;
  sprite_t   *sprite;
  int        head; /* 0..359 */
  int        x;
  int        y;
  int        team;
};


ship_t *create_ship(frame_t *frame, int team, const char *name, 
		    sprite_t *sprite, int head, int x, int y);


#endif /* SHIP_H */
