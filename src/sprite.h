
#ifndef SPRITE_H
#define SPRITE_H


#include <SDL/SDL.h>

#include "misc.h"
#include "frame.h"


#define SPRITE_ROTATE_STEP 20
#define SPRITE_COLOR_DEPTH 32


#if SDL_BYTEORDER == SDL_BIG_ENDIAN

#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff

#else

#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000

#endif


struct sprite_t {
  SDL_Surface *image[360 / SPRITE_ROTATE_STEP];
};


sprite_t *load_sprite(frame_t *frame, SDL_Surface *image);


#endif /* SPRITE_H */
