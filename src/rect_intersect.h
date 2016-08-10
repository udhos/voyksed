
#ifndef RECT_INTERSECT
#define RECT_INTERSECT

#include <SDL/SDL.h>

int rect_intersect(const SDL_Rect *r1, const SDL_Rect *r2, SDL_Rect *overlap);

#endif
