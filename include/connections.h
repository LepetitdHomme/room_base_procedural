#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include "structs.h"

/*                      connections */
void                    connections_append(state_t *state, int src, int dst);
void                    free_connections(state_t *state);
void                    connections_print(state_t *state);

#endif
