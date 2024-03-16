#ifndef GRID_H
#define GRID_H

#include "structs.h"

/*                      grid */
void                    init_grid(state_t *state, int complexity);
void                    free_grid(state_t *state);
void                    reset_grid(state_t *state);

#endif
