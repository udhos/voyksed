
#if 0
SDL_Surface *load_svg(frame_t *frame, const char *name, int alpha)
{
  svg_cairo_t     *svg;
  unsigned int     svg_width;
  unsigned int     svg_height;
  int              bpp    = 32; /* bits per pixel */
  int              btpp   = 4;  /* bytes per pixel */
  int              stride;
  int              space;
  char            *image;
  cairo_surface_t *cairo_surface;
  cairo_t         *cr;
  Uint32           rmask = 0x00ff0000;
  Uint32           gmask = 0x0000ff00;
  Uint32           bmask = 0x000000ff;
  Uint32           amask = 0xff000000;
  SDL_Surface     *sdl_surf_tmp;
  SDL_Surface     *sdl_surf;

  svg_cairo_create(&svg);
  svg_cairo_parse(svg, name);
  svg_cairo_get_size(svg, &svg_width, &svg_height);

  stride = svg_width * btpp;    /* scanline width */
  space  = svg_height * stride; 

  image = malloc(space);
  if (!image) {
    fprintf(stderr, "ugh: malloc(%d)\n", space);
    exit(1);
  }

  cairo_surface = cairo_image_surface_create_for_data(image,
						      CAIRO_FORMAT_ARGB32,
						      svg_width, svg_height,
						      stride);

  cr = cairo_create(cairo_surface);
  /* cairo_scale(cr, 1, 1); */

  svg_cairo_render(svg, cr);

  cairo_destroy(cr);
  cairo_surface_destroy(cairo_surface);
  svg_cairo_destroy(svg);
    
  sdl_surf_tmp = SDL_CreateRGBSurfaceFrom(image, svg_width, svg_height,
					  bpp, stride,
					  rmask, gmask, bmask, amask);

  sdl_surf = alpha ? 
    SDL_DisplayFormatAlpha(sdl_surf_tmp) :
    SDL_DisplayFormat(sdl_surf_tmp);

  if (!sdl_surf) {
    fprintf(stderr, "ugh: unable to convert image: %s: %s\n", 
	    name, SDL_GetError());
    exit(1);
  }
  SDL_FreeSurface(sdl_surf_tmp);

  frame->image_list[frame->image_list_size] = sdl_surf;

  show_image_info(frame, sdl_surf, name, alpha);

  ++frame->image_list_size;

  return sdl_surf;
}
#endif

#if 0
SDL_Surface *load_svg2(frame_t *frame, const char *name, int alpha)
{
  cairo_surface_t      *cairo_surf;
  cairo_status_t        cairo_status;
  cairo_surface_type_t  cairo_surf_type;
  SDL_Surface          *sdl_surf;
  unsigned char        *image_data;

  assert(frame->image_list_size >= 0);
  assert(frame->image_list_size < IMAGE_LIST_MAX);

  {
    char buf[1024];
    snprintf(buf, 1024, "%s/%s", SVG_DIR, name);

    cairo_surf = cairo_svg_surface_create(name, 0, 0);
    cairo_status = cairo_surface_status(cairo_surf);
    if (cairo_status != CAIRO_STATUS_SUCCESS) {
      fprintf(stderr,
	      "ugh: unable to load SVG '%s' from '%s': status=%d: %s\n", 
	      name, buf, cairo_status, cairo_status_to_string(cairo_status));
      exit(1);
    }
    if (!cairo_surf) {
      fprintf(stderr, "ugh: unable to create cairo SVG surface\n");
      exit(1);
    }

    cairo_surf_type = cairo_surface_get_type(cairo_surf);
    
    fprintf(stderr, "cairo svg surf type: %d\n", cairo_surf_type);
  }

  image_data = cairo_image_surface_get_data(cairo_surf);
  if (!image_data) {
    fprintf(stderr, "ugh: unable to get cairo surface data\n");
    exit(1);
  }

  cairo_surface_destroy(cairo_surf);

  assert(0);

  show_image_info(frame, sdl_surf, name, alpha);
    
  return sdl_surf;
}
#endif
