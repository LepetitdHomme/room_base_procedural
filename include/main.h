#ifndef MAIN_H
#define MAIN_H

#include "structs.h"

SDL_Window          *init_sdl();
unsigned int        set_seed(int argc, char *argv[]);
void                clean(state_t *state, SDL_Window *window);

#endif
