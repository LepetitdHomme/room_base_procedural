#ifndef TOOLS_H
#define TOOLS_H

#include "structs.h"

/*                      tools */
void                    print_rect(SDL_Rect rect, char *name);
int                     clamp(int value, int min, int max);
int                     random_int(int lower, int upper);
double                  distance_between_coords(coord_t center_1, coord_t center_2);
double                  squared_distance_between_coords(coord_t center_1, coord_t center_2);
Uint8                   alpha_light(int light, double distance);
coord_t                 extend_segment(coord_t player, coord_t corner);
coord_t                 find_intersection(coord_t p1, coord_t p2, line_t lines[], int size_lines);
coord_t                 find_intersection_with_walls(state_t *state, coord_t player, coord_t corner, enum Octant octant);

#endif
