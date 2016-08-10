
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <SDL/sge.h>

#include "frame.h"
#include "sprite.h"

static void generate_rotations(sprite_t *sprite, SDL_Surface *orig)
{
  int i;
  int j;
  int diag = sqrt(orig->w * orig->w + orig->h * orig->h);

  for (i = 0, j = 0; i < 360; i += SPRITE_ROTATE_STEP, ++j) {

    /* create dst surface for image */
    sprite->image[j] = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 
					    diag, diag, 
					    SPRITE_COLOR_DEPTH, 
					    rmask, gmask, bmask, amask);
    if (!sprite->image[j]) {
      fprintf(stderr, "ugh: %s: %s: SDL_CreateRGBSurface() failed: %s\n", 
	      __FILE__, __PRETTY_FUNCTION__, SDL_GetError());
      exit(1);
    }

    /* rotate image */
    sge_transform(orig, sprite->image[j], i, 1, 1,
		  orig->w >> 1, orig->h >> 1,
		  diag >> 1, diag >> 1, 
		  SGE_TAA | SGE_TSAFE);

  }

}

sprite_t *load_sprite(frame_t *frame, SDL_Surface *image)
{
  sprite_t *sprite;

  assert(frame->sprite_list_size >= 0);
  if (frame->sprite_list_size >= SPRITE_LIST_MAX) {
    fprintf(stderr, "ugh: sprite list max\n");
    exit(1);
  }

  sprite = frame->sprite_list + frame->sprite_list_size;

  generate_rotations(sprite, image);

#if 0
  fprintf(stderr, "sprite (%d x %d) created (%d/%d)\n", 
	  image->w, image->h, frame->sprite_list_size,
	  SPRITE_LIST_MAX);
#endif

  ++frame->sprite_list_size;

  return sprite;
}
