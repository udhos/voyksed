
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ship.h"

ship_t *create_ship(frame_t *frame, int team, const char *name, 
		    sprite_t *sprite, int head, int x, int y)
{
  ship_t *ship;

  assert(frame->ship_list_size >= 0);
  if (frame->ship_list_size >= SHIP_LIST_MAX) {
    fprintf(stderr, "ugh: ship list max\n");
    exit(1);
  }

  ship = frame->ship_list + frame->ship_list_size;

  ship->name   = strdup(name);
  assert(ship->name);
  ship->sprite = sprite;
  ship->head   = head;
  ship->x      = x;
  ship->y      = y;
  ship->team   = team;

#if 0
  fprintf(stderr, "ship '%s' created: team=%d (%d/%d)\n",
	  name, team, frame->ship_list_size,
	  SHIP_LIST_MAX);
#endif

  ++frame->ship_list_size;

  return ship;
}
