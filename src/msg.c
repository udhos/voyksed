
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define __USE_GNU
#define _GNU_SOURCE
#include <stdio.h>

#include "msg.h"

static void msg_shift(frame_t *frame)
{
  assert(frame->msg_list_size >= 0);
  assert(frame->msg_list_size <= MSG_LIST_MAX);

  int i;
  free(frame->msg_list[0]); /* dispose first slot */
  for (i = 1; i < frame->msg_list_size; ++i)
    frame->msg_list[i - 1] = frame->msg_list[i];
  --frame->msg_list_size;
}

void msg_show(frame_t *frame)
{
  /* reset display timer */
  frame->msg_last = time(0);
  frame->msg_begin_index = 0;
}

void msg_write(frame_t *frame, const char *msg)
{
  assert(frame->msg_list_size >= 0);
  assert(frame->msg_list_size <= MSG_LIST_MAX);

  /* if the msg list is full, shift to free space at end */
  if (frame->msg_list_size == MSG_LIST_MAX)
    msg_shift(frame);

  /* put new msg into last slot */
  if (asprintf(&frame->msg_list[frame->msg_list_size], "%s", msg) < 0) {
    fprintf(stderr, "%s: %s(): %d: asprintf() failed\n",
	    __FILE__, __PRETTY_FUNCTION__, __LINE__);
    return;
  }

  ++frame->msg_list_size;

  msg_show(frame);
}

void msg_draw(frame_t *frame, SDL_Surface *screen)
{
  sge_TTFont *font;
  time_t now;
  int i;
  time_t elap;
  int line;

  now = time(0);

  elap = now - frame->msg_last;

  if (elap > MSG_SHOW_TIME) {
    frame->msg_begin_index = elap - MSG_SHOW_TIME;
    if (frame->msg_begin_index >= frame->msg_last)
      return;
  }

  font = frame->font;
  sge_TTF_SetFontSize(font, 13);

  for (i = frame->msg_begin_index, line = 40; 
       i < frame->msg_list_size; ++i, line += 15) {
    sge_tt_textout(screen, font, frame->msg_list[i], 
		   10, line, 0x99CC99, 0, SDL_ALPHA_OPAQUE);
  }
}
