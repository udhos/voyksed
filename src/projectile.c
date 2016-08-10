
#include <stdlib.h>
#include <assert.h>

#include "projectile.h"

projectile_t *projectile_new(frame_t *frame, float x, float y, 
			     float sx, float sy, int fuel,
			     void (*draw)(projectile_t *,
					  frame_t *, 
					  SDL_Surface *),
			     void *draw_arg)
{
  projectile_t *proj;

  assert(frame->projectile_list_size >= 0);
  if (frame->projectile_list_size >= PROJECTILE_LIST_MAX) {
    fprintf(stderr, "ugh: projectile list max\n");
    exit(1);
  }

  proj = frame->projectile_list + frame->projectile_list_size;

  proj->x = x;
  proj->y = y;
  proj->speed_x = sx;
  proj->speed_y = sy;
  proj->fuel = fuel;
  proj->draw = draw;
  proj->draw_arg = draw_arg;

#if 0
  fprintf(stderr, 
	  "created projectile: %d/%d x=%.4f y=%.4f sx=%.4f sy=%.4f fuel=%d\n", 
	  frame->projectile_list_size, PROJECTILE_LIST_MAX,
	  proj->x, proj->y, proj->speed_x, proj->speed_y, proj->fuel);
#endif

  ++frame->projectile_list_size;

  return proj;
}

void projectile_draw_energy(projectile_t *proj, 
			    frame_t *frame, 
			    SDL_Surface *screen)
{
  SDL_Rect dst_rect;

  dst_rect.x = proj->x - frame->x;
  dst_rect.y = proj->y - frame->y;

  if (SDL_BlitSurface(proj->draw_arg, 0, screen, &dst_rect))
    fprintf(stderr, "ugh: projectile blit failed\n");
}

#define SQUARE(x) ((x) * (x))

void projectile_step(projectile_t *proj, frame_t *frame, SDL_Surface *screen)
{
  if (proj->fuel <= 0)
    return;

  /* invert speed if too far from center */
  if ((SQUARE(abs(proj->x - (frame->field_width >> 1))) +
       SQUARE(abs(proj->y - (frame->field_height >> 1))))
      > 40000) {
    proj->speed_x = -proj->speed_x;
    proj->speed_y = -proj->speed_y;
  }

  proj->x += proj->speed_x;
  proj->y += proj->speed_y;

  proj->draw(proj, frame, screen);

  --proj->fuel;
}
