
#include "rect_intersect.h"
#include "misc.h"

int rect_intersect(const SDL_Rect *r1, const SDL_Rect *r2, SDL_Rect *overlap)
{
  int r1_x2;
  int r1_y2;
  int r2_x2;
  int r2_y2;
  
  /* check if intersection is possible */

  /* is r1 above r2 ? */
  r1_y2 = r1->y + r1->h - 1;
  if (r1_y2 < r2->y)
    return 0;

  /* is r1 below r2 ? */
  r2_y2 = r2->y + r2->h - 1;
  if (r1->y > r2_y2)
    return 0;

  /* is r1 to the left of r2 ? */
  r1_x2 = r1->x + r1->w - 1;
  if (r1_x2 < r2->x)
    return 0;

  /* is r1 to the right of r2 ? */
  r2_x2 = r2->x + r2->w - 1;
  if (r1->x > r2_x2)
    return 0;

  /* need return overlap ? */
  if (!overlap)
    return -1;

  /* find overlap */

  overlap->x = MAX(r1->x, r2->x);
  overlap->y = MAX(r1->y, r2->y);
  overlap->w = MIN(r1_x2, r2_x2) - overlap->x + 1;
  overlap->h = MIN(r1_y2, r2_y2) - overlap->y + 1;

  return -1;
}
