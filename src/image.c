
#include <assert.h>
#include <stdlib.h>

#include <SDL/SDL_image.h>

#include "frame.h"
#include "image.h"


const char * const IMAGE_DIR = DATA_DIR "/image";


void show_image_info(frame_t *frame, SDL_Surface *image,
		     const char *name, int alpha)
{
  fprintf(stderr, "image %d/%d '%s' (%d x %d) loaded "
	  "alpha=%d hwsurf=%d hwaccel=%d\n", 
	  frame->image_list_size, IMAGE_LIST_MAX,
	  name, image->w, image->h,
	  alpha,
	  (image->flags & SDL_HWSURFACE), (image->flags & SDL_HWACCEL));
}

SDL_Surface *load_image(frame_t *frame, const char *name, int alpha)
{
  SDL_Surface *raw_image;
  SDL_Surface *image;

  assert(frame->image_list_size >= 0);
  assert(frame->image_list_size < IMAGE_LIST_MAX);

  {
    char buf[1024];
    snprintf(buf, 1024, "%s/%s", IMAGE_DIR, name);
    raw_image = IMG_Load(buf);
    if (!raw_image) {
      fprintf(stderr, "ugh: unable to load image '%s' from '%s': %s\n", 
	      name, buf, SDL_GetError());
      exit(1);
    }
  }

  if (alpha)
    image = SDL_DisplayFormatAlpha(raw_image);
  else
    image = SDL_DisplayFormat(raw_image);

  if (!image) {
    fprintf(stderr, "ugh: unable to convert image: %s: %s\n", 
	    name, SDL_GetError());
    exit(1);
  }
  SDL_FreeSurface(raw_image);

  frame->image_list[frame->image_list_size] = image;

  show_image_info(frame, image, name, alpha);

  ++frame->image_list_size;

  return image;
}

