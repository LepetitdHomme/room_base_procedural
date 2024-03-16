#ifndef INPUTS_H
#define INPUTS_H

#include "structs.h"

/*                      inputs */
void                    inputs_handle(state_t *state, int *quit, SDL_Event *event, int *inputs_state);
void                    inputs_keydown(state_t *state, SDL_Event *event, int *inputs_state);

#endif
