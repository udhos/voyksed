#
# $Id$
#

CC                = gcc
CAIRO_CFLAGS      = $(shell pkg-config cairo --cflags)
CAIRO_SVG_CFLAGS  = $(shell pkg-config librsvg-2.0 --cflags)
CFLAGS            = -Wall -O3 -ggdb $(CAIRO_CFLAGS) $(CAIRO_SVG_CFLAGS)
CAIRO_LDFLAGS     =  $(shell pkg-config cairo --libs)
CAIRO_SVG_LDFLAGS =  $(shell pkg-config librsvg-2.0 --libs)
#LDFLAGS          = -lSDL -lSDL_image -lSGE -lSDL_gfx
LDFLAGS           = -lSDL -lSDL_image -lSGE \
			$(CAIRO_LDFLAGS) $(CAIRO_SVG_LDFLAGS)
TARGET            = voyksed
OBJ               = rect_intersect.o font.o image.o sprite.o ship.o msg.o \
			  projectile.o video.o game.o svg.o main.o


.PHONY: default
default: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o *~ .*~ $(TARGET)

.PHONY: build
build: clean default

voyksed: $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

