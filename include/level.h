#ifndef LEVEL_H
#define LEVEL_H

#include "structs.h"

/*                      level */
void                    init_level(state_t *state, int complexity);
int           					max_rect_side(int x, int y, int n);
void                    node_to_grid(state_t *state, graph_t *node, int with_parent, int with_children, int elevation);
void                    level_to_grid(state_t *state, graph_t *node);

#endif
