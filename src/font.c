
#include <stdlib.h>

#include <SDL/sge.h>

#include "font.h"
#include "misc.h"


const char * const FONT_DIR = DATA_DIR "/font";


sge_TTFont *font_load(const char *file, int ptsize)
{
  char buf[1024];
  sge_TTFont *font;

  snprintf(buf, 1024, "%s/%s", FONT_DIR, file);

  font = sge_TTF_OpenFont(buf, ptsize);
  if (!font) {
    fprintf(stderr, "unable to load font: %s: %s\n", file, 
	    SDL_GetError());
    exit(1);
  } 

  return font;
}


