
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/sge.h>

#include "game.h"
#include "frame.h"
#include "image.h"
#include "projectile.h"
#include "video.h"
#include "rect_intersect.h"
#include "msg.h"
#include "font.h"
#include "svg.h"

static SDL_Surface* frame_bg_tile(frame_t *frame, int x, int y)
{
#if 0
  fprintf(stderr, 
	  "FIXME: frame_bg_file(): get background tile for index (%d,%d)\n",
	  x, y);
#endif
  return frame->bg_tile;
}

static void background_update(frame_t *frame, SDL_Surface *background)
{
  int bg_tile_w = frame->bg_tile->w;
  int bg_tile_h = frame->bg_tile->h;

  /*
   * find indexes of tiles which may
   * intersect with frame
   */
  int tile_x1 = frame->x / bg_tile_w; /* column of upper leftmost tile */
  int tile_y1 = frame->y / bg_tile_h; /* row of upper leftmost tile */
  int tile_x2 = (frame->x + frame->w - 1) / bg_tile_w;
  int tile_y2 = (frame->y + frame->h - 1) / bg_tile_h;

  SDL_Rect bg_edge;
  SDL_Rect tile_edge;
  int x;
  int y;

  /* background border */
  bg_edge.x = frame->x;
  bg_edge.y = frame->y;
  bg_edge.w = frame->w;
  bg_edge.h = frame->h;

  /* size of tile border */
  tile_edge.w = bg_tile_w;
  tile_edge.h = bg_tile_h;

  /*
   * scan selected tiles
   */
  for (y = tile_y1; y <= tile_y2; ++y) {
    for (x = tile_x1; x <= tile_x2; ++x) {

      SDL_Surface *bg_tile = frame_bg_tile(frame, x, y);
      SDL_Rect overlap;

      /* current tile position */
      tile_edge.x = x * bg_tile_w;
      tile_edge.y = y * bg_tile_h;

      /* tile overlaps with frame? */
      if (rect_intersect(&bg_edge, &tile_edge, &overlap)) {
	SDL_Rect src, dst;

	/* overlap relative to tile */
	src.x = overlap.x - tile_edge.x;
	src.y = overlap.y - tile_edge.y;
	src.w = overlap.w;
	src.h = overlap.h;

	/* overlap relative to frame */
	dst.x = overlap.x - bg_edge.x;
	dst.y = overlap.y - bg_edge.y;

	/* redraw background */
	if (SDL_BlitSurface(bg_tile, &src, background, &dst))
	  fprintf(stderr, 
		  "ugh: SDL_BlitSurface() failed: tile into background\n");

      }
    }
  }
}

static void init_frame(frame_t *frame, int do_delay, int clip)
{
  const char *bg_file = "earth.jpg";
  const char *font_file = "font.ttf";
  const float BOLTS = 500;

  frame->clip_ships = clip;
  frame->do_delay   = do_delay;
  frame->delay      = 50; /* adjusted later */

  fprintf(stderr, "clip ships: %d\n",
	  frame->clip_ships);

  /*
   * define size of the game field
   */
  frame->field_width = 5000;
  frame->field_height = 5000;

  fprintf(stderr, "field size: %d x %d\n",
	  frame->field_width, frame->field_height);

  /*
   * we copy the screen dimensions on the
   * frame for convenience
   */
  frame->w = frame->screen->w;
  frame->h = frame->screen->h;

  fprintf(stderr, "frame size: %d x %d\n",
	  frame->w, frame->h);

  /*
   * put frame at center of field
   */
  frame->x = (frame->field_width - frame->w) >> 1;
  frame->y = (frame->field_height - frame->h) >> 1;

  /*
   * init lists
   */
  frame->image_list = malloc(IMAGE_LIST_MAX * sizeof(SDL_Surface*));
  assert(frame->image_list);
  frame->image_list_size = 0;

  frame->sprite_list = malloc(SPRITE_LIST_MAX * sizeof(sprite_t));
  assert(frame->sprite_list);
  frame->sprite_list_size = 0;

  frame->ship_list = malloc(SHIP_LIST_MAX * sizeof(ship_t));
  assert(frame->ship_list);
  frame->ship_list_size = 0;

  frame->projectile_list = malloc(PROJECTILE_LIST_MAX * sizeof(projectile_t));
  assert(frame->projectile_list);
  frame->projectile_list_size = 0;

  frame->msg_list_size = 0;
  frame->msg_last = 0;
  frame->msg_begin_index = -1;

  /*
   * load tile
   */
  frame->bg_tile = load_image(frame, bg_file, 0);

  /*
   * load font
   */
  frame->font = font_load(font_file, 30);

  /*
   * write on the tile
   */
  sge_tt_textout(frame->bg_tile, frame->font, "Voyksed", 30,50, 255, 0, 120);
  sge_TTF_SetFontSize(frame->font, 20);
  sge_TTF_SetFontStyle(frame->font, SGE_TTF_ITALIC|SGE_TTF_UNDERLINE);
  sge_tt_textout(frame->bg_tile, frame->font, "Hello World!", 
		 30,150, (155 << 16) + (200 << 8) + 100, 0, SDL_ALPHA_OPAQUE);
  sge_TTF_SetFontStyle(frame->font, 0);

  {
    int i;
    int team0 = 0;
    int team1 = 1;

    SDL_Surface *img_scout   = load_image(frame, "scout.png", 1);
    SDL_Surface *img_fighter = load_image(frame, "fighter.png", 1);
    SDL_Surface *img_plasma1 = load_image(frame, "torpedo.png", 1);
    SDL_Surface *img_plasma2 = load_image(frame, "plasma_green.png", 1);
    SDL_Surface *img_capital = load_svg(frame, "capital_ship.svg", 1);

    sprite_t *scout   = load_sprite(frame, img_scout);
    sprite_t *fighter = load_sprite(frame, img_fighter);
    sprite_t *capital = load_sprite(frame, img_capital);

    int x = frame->x + 50;
    int y = frame->y + 50;

    create_ship(frame, team0, "cruiser", capital, 0, x + 300, y + 300);

    create_ship(frame, team0, "alpha_scout1",   scout,   0, x,       y);
    create_ship(frame, team0, "alpha_scout2",   scout,   0, x + 50,  y);
    create_ship(frame, team0, "alpha_scout3",   scout,   0, x + 100, y);
    create_ship(frame, team0, "alpha_scout4",   scout,   0, x + 150, y);
    create_ship(frame, team0, "alpha_scout5",   scout,   0, x + 200, y);
    create_ship(frame, team0, "alpha_fighter1", fighter, 0, x + 250, y);
    create_ship(frame, team0, "alpha_fighter2", fighter, 0, x + 300, y);
    create_ship(frame, team0, "alpha_fighter3", fighter, 0, x + 350, y);
    create_ship(frame, team0, "alpha_fighter4", fighter, 0, x + 400, y);
    create_ship(frame, team0, "alpha_fighter5", fighter, 0, x + 450, y);

    y += 100;

    create_ship(frame, team0, "alpha_scout6",    scout,   0, x,       y);
    create_ship(frame, team0, "alpha_scout7",    scout,   0, x + 50,  y);
    create_ship(frame, team0, "alpha_scout8",    scout,   0, x + 100, y);
    create_ship(frame, team0, "alpha_scout9",    scout,   0, x + 150, y);
    create_ship(frame, team0, "alpha_scout10",   scout,   0, x + 200, y);
    create_ship(frame, team0, "alpha_fighter6",  fighter, 0, x + 250, y);
    create_ship(frame, team0, "alpha_fighter7",  fighter, 0, x + 300, y);
    create_ship(frame, team0, "alpha_fighter8",  fighter, 0, x + 350, y);
    create_ship(frame, team0, "alpha_fighter9",  fighter, 0, x + 400, y);
    create_ship(frame, team0, "alpha_fighter10", fighter, 0, x + 450, y);

    y += 100;

    create_ship(frame, team1, "gold_scout1",   scout,   0, x,       y);
    create_ship(frame, team1, "gold_scout2",   scout,   0, x + 50,  y);
    create_ship(frame, team1, "gold_scout3",   scout,   0, x + 100, y);
    create_ship(frame, team1, "gold_scout4",   scout,   0, x + 150, y);
    create_ship(frame, team1, "gold_scout5",   scout,   0, x + 200, y);
    create_ship(frame, team1, "gold_fighter1", fighter, 0, x + 250, y);
    create_ship(frame, team1, "gold_fighter2", fighter, 0, x + 300, y);
    create_ship(frame, team1, "gold_fighter3", fighter, 0, x + 350, y);
    create_ship(frame, team1, "gold_fighter4", fighter, 0, x + 400, y);
    create_ship(frame, team1, "gold_fighter5", fighter, 0, x + 450, y);

    y += 100;

    create_ship(frame, team1, "gold_scout6",    scout,   0, x,       y);
    create_ship(frame, team1, "gold_scout7",    scout,   0, x + 50,  y);
    create_ship(frame, team1, "gold_scout8",    scout,   0, x + 100, y);
    create_ship(frame, team1, "gold_scout9",    scout,   0, x + 150, y);
    create_ship(frame, team1, "gold_scout10",   scout,   0, x + 200, y);
    create_ship(frame, team1, "gold_fighter6",  fighter, 0, x + 250, y);
    create_ship(frame, team1, "gold_fighter7",  fighter, 0, x + 300, y);
    create_ship(frame, team1, "gold_fighter8",  fighter, 0, x + 350, y);
    create_ship(frame, team1, "gold_fighter9",  fighter, 0, x + 400, y);
    create_ship(frame, team1, "gold_fighter10", fighter, 0, x + 450, y);


    for (i = 0; i < BOLTS; ++i)
      projectile_new(frame, 
		     frame->field_width >> 1, 
		     frame->field_height >> 1,
		     3 * cos(i * (360 / BOLTS) * (M_PI / 180)), 
		     3 * -sin(i * (360 / BOLTS) * (M_PI / 180)), 
		     5000,
		     projectile_draw_energy,
		     img_plasma1);

    for (i = 0; i < BOLTS; ++i)
      projectile_new(frame, 
		     (frame->field_width >> 1) + 40, 
		     (frame->field_height >> 1) + 20,
		     3 * cos(i * (360 / BOLTS) * (M_PI / 180)), 
		     3 * -sin(i * (360 / BOLTS) * (M_PI / 180)), 
		     5000,
		     projectile_draw_energy,
		     img_plasma2);
  }

  /*
   * init profiling
   */
  frame->ts = time(0);
  frame->updates = 0;
  frame->update_rate[0] = '\0';
}

static void draw_ships(frame_t *frame, SDL_Surface *screen)
{
  SDL_Rect dst_rect;
  ship_t *ship;
  SDL_Surface *image;
  int i;
  SDL_Rect frame_rect; 

  if (frame->clip_ships) {
    frame_rect.x = frame->x;
    frame_rect.y = frame->y;
    frame_rect.w = frame->w;
    frame_rect.h = frame->h;
  }

  for (i = 0; i < frame->ship_list_size; ++i) {

    ship = frame->ship_list + i;

    ship->head += SPRITE_ROTATE_STEP;
    ship->head %= 360; 

    image = ship->sprite->image[ship->head / SPRITE_ROTATE_STEP];

    if (frame->clip_ships) {
      SDL_Rect ship_rect;

      ship_rect.x = ship->x;
      ship_rect.y = ship->y;
      ship_rect.w = image->w;
      ship_rect.h = image->h;

      if (!rect_intersect(&frame_rect, &ship_rect, 0))
	continue; /* skip to next ship */
    }

    dst_rect.x = ship->x - frame->x;
    dst_rect.y = ship->y - frame->y;

#if 0
    fprintf(stderr, "frame: x=%d y=%d\n",
	    frame->x, frame->y);

    fprintf(stderr, "ship: abs: x=%d y=%d rel: x=%d y=%d\n",
	    ship->x, ship->y, dst_rect.x, dst_rect.y);
#endif

    if (SDL_BlitSurface(image, NULL, screen, &dst_rect))
      fprintf(stderr, "ugh: blit ship failed\n");
  }
}

static void move_projectiles(frame_t *frame, SDL_Surface *screen)
{
  projectile_t *proj;
  int i;

  for (i = 0; i < frame->projectile_list_size; ++i) {
    proj = frame->projectile_list + i;
    projectile_step(proj, frame, screen);
  }
}

#if 0
static void draw_circles(frame_t *frame, SDL_Surface *screen)
{
  int r;
  int c;

  for (r = 50, c = 200; r <= 200; r += 50, c -= 10) {
    int rgb = (c << 24) + (c << 16) + (c << 8);
    sge_Circle(screen, frame->w >> 1, frame->h >> 1, r, rgb);
  }
}
#endif

static void draw_frame_rate(frame_t *frame, SDL_Surface *screen)
{
  time_t now = time(0);
  sge_TTFont *font = frame->font;

  if (now > frame->ts) {
    char buf[1024];

    snprintf(frame->update_rate, sizeof(frame->update_rate),
	     "fps=%ld delay=%dms coord: %dx%d",
	     frame->updates, frame->delay,
	     frame->x, frame->y);
    
    if (frame->do_delay) {
      if (frame->updates > 40) {
	++frame->delay;
	snprintf(buf, 1024, "too high fps=%ld, increasing delay: %d",
		 frame->updates, frame->delay);
	msg_write(frame, buf);
      } 
      else if (frame->updates < 30) {
	if (frame->delay > 0) {
	  --frame->delay;
	  snprintf(buf, 1024, "too low fps=%ld, decreasing delay: %d",
		   frame->updates, frame->delay);
	  msg_write(frame, buf);
	}
      }
    }
    
    frame->ts = now;
    frame->updates = 0;
  }
  sge_TTF_SetFontSize(frame->font, 15);
  sge_tt_textout(screen, font, frame->update_rate, 
		 10,20, 0xAAAAAA, 0, SDL_ALPHA_OPAQUE);

  ++frame->updates;
}

static void frame_draw(frame_t *frame, SDL_Surface *screen)
{
  draw_ships(frame, screen);

  move_projectiles(frame, screen);

  /* draw_circles(frame, screen); */

  msg_draw(frame, screen);

  draw_frame_rate(frame, screen);
}

static void frame_update(frame_t *frame)
{
  SDL_Surface *screen = frame->screen;

  background_update(frame, screen);

  frame_draw(frame, screen);

  if (SDL_Flip(screen)) {
    fprintf(stderr, "ugh: SDL_Flip() failed\n");
    exit(1);
  }
}

static void game_loop(frame_t *frame)
{
  SDL_Event event;
  SDLMod mod;

  for (;;) {
    frame_update(frame);

    if (frame->do_delay)
      SDL_Delay(frame->delay);

    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
      break;

    mod = SDL_GetModState();
    if (mod & KMOD_SHIFT)
      msg_show(frame);

    if (event.type == SDL_KEYDOWN) {

      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
	return;

      case SDLK_LEFT:
	frame->x -= 50;
	if (frame->x < 0)
	  frame->x = 0;
	break;

      case SDLK_RIGHT:
	frame->x += 50;
	if (frame->x > frame->field_width - frame->w - 1)
	  frame->x = frame->field_width - frame->w - 1;
	break;

      case SDLK_UP:
	frame->y -= 50;
	if (frame->y < 0)
	  frame->y = 0;
	break;

      case SDLK_DOWN:
	frame->y += 50;
	if (frame->y > frame->field_height - frame->h - 1)
	  frame->y = frame->field_height - frame->h - 1;
	break;

      default:
	break;
      }

    }
  }
}

void game_run(Uint32 screen_flags, int video_mode, int do_delay, int clip)
{
  frame_t frame;

  frame.screen = set_video_mode(screen_flags, video_mode);

  svg_init();

  init_frame(&frame, do_delay, clip);

  svg_term();

  game_loop(&frame);
}
