
#include <assert.h>
#include <stdlib.h>

#include <cairo.h>
#include <librsvg/rsvg.h>
//#include <librsvg/rsvg-cairo.h>

#include "frame.h"
#include "svg.h"
#include "image.h"


const char * const SVG_DIR = DATA_DIR "/svg";

void svg_init()
{
  //rsvg_init(); // deprecated
}

void svg_term()
{
  //rsvg_term(); // deprecated
}

/*
  Based on examples from:

  http://lists.cairographics.org/archives/cairo/2007-March/010035.html

  http://www.tuxpaint.org/presentations/lugod-2007-06/page6.html

  http://webcvs.cairographics.org/cairo-demo/sdl/main.c
 */

SDL_Surface *load_svg(frame_t *frame, const char *name, int alpha)
{
  unsigned int       width;
  unsigned int       height;
  int                bpp    = 32; /* bits per pixel */
  int                btpp   = 4;  /* bytes per pixel */
  int                stride;
  int                size;
  unsigned char     *cairo_data;
  cairo_surface_t   *cairo_surface;
  cairo_t           *cr;
  RsvgDimensionData  g_dimension;
  RsvgHandle        *rsvg_handle;
  SDL_Surface       *sdl_surf_tmp;
  SDL_Surface       *sdl_surf;

  {
    char    buf[1024];
    GError *e = NULL;

    snprintf(buf, 1024, "%s/%s", SVG_DIR, name);

    rsvg_handle = rsvg_handle_new_from_file(buf, &e);
    if (rsvg_handle == NULL) {
      fprintf(stderr,
	      "ugh: #1 unable to load SVG '%s' from '%s': null RSVG handler\n", 
	      name, buf);
    }
    if (e != NULL) {
      fprintf(stderr,
	      "ugh: #2 unable to load SVG '%s' from '%s': %s\n", 
	      name, buf, e->message);
      exit(1);
    }

    rsvg_handle_get_dimensions(rsvg_handle, &g_dimension);
    width = g_dimension.width;
    height = g_dimension.height;
  }

  stride = width * btpp;    /* scanline width */
  size = height * stride; 

  cairo_data = malloc(size);
  if (!cairo_data) {
    fprintf(stderr, "ugh: malloc(%d) failed\n", size);
    exit(1);
  }

  cairo_surface =
    cairo_image_surface_create_for_data(cairo_data,
					CAIRO_FORMAT_ARGB32,
					width, height,
					stride);

  cr = cairo_create(cairo_surface);
  cairo_save(cr);

#if 0
  cairo_set_source_rgb(cr, 0.0, 1.0, 1.0); /* cyan */
#endif
  cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
  cairo_paint(cr);

  rsvg_handle_render_cairo(rsvg_handle, cr);

#if 0
  cairo_surface_write_to_png(cairo_get_target(cr), "/tmp/test.png");
#endif

  sdl_surf_tmp = SDL_CreateRGBSurfaceFrom(cairo_data,
					  width, height,
					  bpp, stride,
					  0x00ff0000, 0x0000ff00, 
					  0x000000ff, 0xff000000);

  sdl_surf = alpha ? 
    SDL_DisplayFormatAlpha(sdl_surf_tmp) :
    SDL_DisplayFormat(sdl_surf_tmp);

  if (!sdl_surf) {
    fprintf(stderr, "ugh: unable to convert image: %s: %s\n", 
	    name, SDL_GetError());
    exit(1);
  }
  SDL_FreeSurface(sdl_surf_tmp);

  cairo_destroy(cr);

  frame->image_list[frame->image_list_size] = sdl_surf;

  show_image_info(frame, sdl_surf, name, alpha);

  ++frame->image_list_size;

  return sdl_surf;
}
