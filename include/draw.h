#ifndef DRAW_H
#define DRAW_H

#include "structs.h"

/*                      draw */
void                    draw_grid(state_t *state);
void                    draw_map_grid(state_t *state);
void                    draw_node(state_t *state);
void                    draw_entities(state_t *state);
void                    draw_clamp_scroll(state_t *state);
void                    draw_update_scroll(state_t *state);
void                    draw_compute_screen_sizes(state_t *state);

#endif
