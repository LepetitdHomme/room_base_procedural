#ifndef LIGHT_MAP_H
#define LIGHT_MAP_H

#include "structs.h"

/*                      light_map */
void                    init_light_map(graph_t *node);
void                    free_light_map(graph_t *node);
void                    reset_light_map(graph_t *node);
void                    update_light_map(state_t *state);
void                    diffuse_light_map(state_t *state);
void                    print_light_map(graph_t *node);

#endif
