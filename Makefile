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
# for SDL2_image:
# 	https://github.com/libsdl-org/SDL_image/releases

UNAME_S := $(shell uname -s)

CC := gcc
CFLAGS := -I/include -Wall -Wextra
LDFLAGS :=
LIBS=

SRC_DIR := src
OBJ_DIR := obj

# List of source files
SRC := $(wildcard $(SRC_DIR)/*.c)

# Generate list of object files
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

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
	LIBS+=-lSDL2_image -lm
endif

# Targets
EXECUTABLE := play

# Compile rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link rule
$(EXECUTABLE): $(OBJ)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean re

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

re: clean $(EXECUTABLE)
