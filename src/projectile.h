
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL/SDL.h>

#include "frame.h"


struct projectile_t {
  float x;
  float y;
  float speed_x;
  float speed_y;
  int fuel;
  void (*draw)(projectile_t *, frame_t *, SDL_Surface *);
  void *draw_arg;
};


projectile_t *projectile_new(frame_t *frame, 
			     float x, float y, 
			     float sx, float sy,
			     int fuel,
			     void (*draw)(projectile_t *,
					  frame_t *, 
					  SDL_Surface *),
			     void *draw_arg);

void projectile_draw_energy(projectile_t *proj, 
			    frame_t *frame, 
			    SDL_Surface *screen);

void projectile_step(projectile_t *proj, frame_t *frame, SDL_Surface *screen);


#endif /* PROJECTILE_H */

