#ifndef TYPE_H
#define TYPE_H

#include "structs.h"

/*                      type */
SDL_Color               type_to_map_color(enum Type type);
int                     is_door_type(enum Type type);
int                     type_stops_light(enum Type type);
int                     is_floor_type(enum Type type);
int                     is_wall_type(enum Type type);
enum Octant             corner_to_octant(enum Type corner);
enum Octant             wall_to_octant(coord_t src, coord_t dst);
int                     direction_to_degrees(enum Dir dir);
enum Dir                invert_dir(enum Dir dir);
enum Type               door_dir_to_type(enum Dir dir);
SDL_Color               pick_color(state_t *state, int i, int j);
SDL_RendererFlip        flip_from_type(enum Type type);
double                  angle_from_type(enum Type type);
enum Type               wall_type(SDL_Rect room, int x, int y);

#endif
