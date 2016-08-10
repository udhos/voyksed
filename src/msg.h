
#ifndef MSG_H
#define MSG_H


#include "frame.h"


#define MSG_SHOW_TIME  5 /* seconds */


void msg_write(frame_t *frame, const char *msg);
void msg_draw(frame_t *frame, SDL_Surface *screen);
void msg_show(frame_t *frame);


#endif /* MSG_H */
