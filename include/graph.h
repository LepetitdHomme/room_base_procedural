#ifndef GRAPH_H
#define GRAPH_H

#include "structs.h"

/*                      graph */
void                    graph_create(state_t *state);
graph_t                 *graph_create_node(SDL_Rect rect, coord_t center, int is_corridor, int elevation, int id);
graph_t                 *graph_create_node_from_connection(state_t *state, graph_t *parent_node, graph_t *child_node);
graph_t                 *graph_create_node_from_room(state_t *state, con_t *connections, int num_connections, room_t *room);
void                    graph_add_child(graph_t *parent, graph_t *child);
void                    free_node(graph_t *node);
void                    free_graph(state_t *state);
void                    graph_print(graph_t *node, int depth);

#endif
