#ifndef DOOR_H
#define DOOR_H

#include "structs.h"

/*                      door */
void                    open_door(state_t *state);
void                    doors_append(graph_t *src_node, door_t door_node);
void                    free_doors(graph_t *node);
enum Dir                door_dir(SDL_Rect room, int x, int y);
coord_t                 next_coord_with_step(coord_t src, enum Dir dir);
door_t                  door_coordinates(graph_t *src, graph_t *dst);
SDL_Rect                rect_from_doors(door_t src, door_t dst);

#endif
