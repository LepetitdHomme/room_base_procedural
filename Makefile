# Makefile
# for SDL:
#   follow https://wiki.libsdl.org/SDL2/Installation
#  clone somewhere else :)
#   git clone https://github.com/libsdl-org/SDL.git -b SDL2
#   cd SDL
#   mkdir build
#   cd build
#   ../configure
#   make
#   sudo make install

UNAME_S := $(shell uname -s)

CC=gcc
CFLAGS=-I/includes
LDFLAGS=
LIBS=

DEPS = common.h
OBJ = main.o grid.o room.o tools.o level.o ll_rooms.o kruskal.o player.o textures.o

# macOS configuration
ifeq ($(UNAME_S),Darwin)
	CFLAGS+=-I/opt/homebrew/Cellar/sdl2/2.30.0/include
	LDFLAGS+=-L/opt/homebrew/Cellar/sdl2/2.30.0/lib
	LIBS+=-lSDL2
endif

# Linux configuration
ifeq ($(UNAME_S),Linux)
	CFLAGS+=`sdl2-config --cflags`
	LDFLAGS+=`sdl2-config --libs`
	LIBS+=-lSDL2_ttf -lm
endif

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

play: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o play

re: clean play
