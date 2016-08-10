
#ifndef FRAME_H
#define FRAME_H

#include <time.h>

#include <SDL/SDL.h>
#include <SDL/sge.h>

#include "sprite.h"
#include "ship.h"


/*
  frame represents the game window visible for the user
 */


#define IMAGE_LIST_MAX      20
#define SPRITE_LIST_MAX     10
#define SHIP_LIST_MAX       50
#define PROJECTILE_LIST_MAX 1000
#define MSG_LIST_MAX        10


struct frame_t {
  SDL_Surface  *screen;         /* hardware */
  SDL_Surface  *bg_tile;        /* background tile */
  sge_TTFont   *font;
  int          field_width;     /* size of field */
  int          field_height;    /* size of field */
  int          x;               /* position of frame in the field */
  int          y;               /* position of frame in the field */
  int          w;               /* size of frame (copy from screen) */
  int          h;               /* size of frame (copy from screen) */
  time_t       ts;              /* profiling */
  size_t       updates;         /* profiling */
  char         update_rate[40]; /* profiling */
  int          delay;           /* automatic frame rate tuning */
  int          do_delay;        /* automatic frame rate tuning */
  int          flip;            /* flip instead of blit? */

  int          clip_ships;      /* draw only ships within frame */

  SDL_Surface  **image_list;
  int          image_list_size;

  sprite_t     *sprite_list;
  int          sprite_list_size;

  ship_t       *ship_list;
  int          ship_list_size;

  projectile_t *projectile_list;
  int          projectile_list_size;

  char         *msg_list[MSG_LIST_MAX];
  int          msg_list_size;
  time_t       msg_last;
  int          msg_begin_index;
};


#endif /* FRAME_H */
