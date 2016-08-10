
#ifndef IMAGE_H
#define IMAGE_H


#include <SDL/SDL.h>


extern const char * const IMAGE_DIR;


void show_image_info(frame_t *frame, SDL_Surface *image,
		     const char *name, int alpha);
SDL_Surface *load_image(frame_t *frame, const char *name, int alpha);


#endif /* IMAGE_H */


