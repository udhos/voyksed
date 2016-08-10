
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include <SDL/SDL.h>

#include "video.h"

static void dump_video_info()
{
  const SDL_VideoInfo *video_info;

  video_info = SDL_GetVideoInfo();
  fprintf(stderr, "hardware support available: %d\n", 
	  video_info->hw_available);
  fprintf(stderr, "window manager available: %d\n", 
	  video_info->wm_available);
  fprintf(stderr, "hw2hw blits accelerated: %d\n", 
	  video_info->blit_hw);
  fprintf(stderr, "hw2hw colorkey blits accelerated: %d\n", 
	  video_info->blit_hw_CC);
  fprintf(stderr, "hw2hw alpha blits accelerated: %d\n", 
	  video_info->blit_hw_A);
  fprintf(stderr, "sw2hw blits accelerated: %d\n", 
	  video_info->blit_sw);
  fprintf(stderr, "sw2hw colorkey blits accelerated: %d\n", 
	  video_info->blit_sw_CC);
  fprintf(stderr, "sw2hw alpha blits accelerated: %d\n", 
	  video_info->blit_sw_A);
  fprintf(stderr, "color fills accelerated: %d\n", 
	  video_info->blit_fill);
  fprintf(stderr, "video memory: %d Kb\n", 
	  video_info->video_mem);
}

SDL_Surface *set_video_mode(Uint32 flags, int video_mode)
{
  SDL_Surface *screen;
  int width;
  int height;

  switch (video_mode) {
  case 0:
    width  = 640;
    height = 480;
    break;
  case 1:
    width  = 800;
    height = 600;
    break;
  default:
    width  = 1024;
    height = 768;
  }

  int depth  = 32;

  screen = SDL_SetVideoMode(width, height, depth, flags);
  if (screen == NULL) {
    fprintf(stderr, "environmental SDL_VIDEODRIVER = %s\n", SDL_GetError());
    exit(1);
  }

  {
    const int BUFSZ = 64;
    char buf[BUFSZ];

    if (SDL_VideoDriverName(buf, BUFSZ)) {
      fprintf(stderr, "video driver: %s\n", buf);

      if (!strcmp(buf, "dga")) {
	uid_t uid = geteuid();
	fprintf(stderr, "uid: %d\n", uid);
	if (uid) {
	  fprintf(stderr, "dga driver requires root privilege\n");
	  exit(1);
	}
      }
    }
    else
      fprintf(stderr, "ugh: could not get video driver name\n");
  }

  dump_video_info();

  return screen;
}
