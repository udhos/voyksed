/*
  $Id$
 */

#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/sge.h>

#include "game.h"

static const char *prog_name;

static void usage()
{
  fprintf(stderr, "usage: %s [-h] [-c] [-m] [-full] [-nodelay]\n", prog_name);
}

int main(int argc, char *argv[])
{
  Uint32 screen_flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
  int i;
  int video_mode = 0;
  int do_delay = 1;
  int clip = 0;

  prog_name = argv[0];

  for (i = 1; i < argc; ++i) {
    const char *arg = argv[i];

    if (!strcmp(arg, "-h")) {
      usage();
      exit(0);
    }

    if (!strcmp(arg, "-c")) {
      clip = 1;
      continue;
    }

    if (!strcmp(arg, "-m")) {
      ++video_mode;
      continue;
    }

    if (!strcmp(arg, "-full")) {
      screen_flags |= SDL_FULLSCREEN;
      continue;
    }

    if (!strcmp(arg, "-nodelay")) {
      do_delay = 0;
      continue;
    }

    fprintf(stderr, "%s: unknown argument: %s\n", prog_name, arg);
    usage();
    exit(1);
  }

  if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "%s: unable to init SDL: %s\n", 
	    prog_name, SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  if (sge_TTF_Init()) {
    fprintf(stderr, "%s: unable to init TT font: %s\n",
	    prog_name, SDL_GetError());
    exit(1);
  } 	

  game_run(screen_flags, video_mode, do_delay, clip);

  exit(0);
}
