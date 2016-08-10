
#ifndef SVG_H
#define SVG_H


#include <SDL/SDL.h>


extern const char * const SVG_DIR;

void svg_init();
void svg_term();

SDL_Surface *load_svg(frame_t *frame, const char *name, int alpha);


#endif /* SVG_H */


