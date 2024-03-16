#ifndef ROOM_H
#define ROOM_H

#include "structs.h"

/*                      room */
void                    tmp_fill_grid(state_t *state, SDL_Rect rect);
int                     is_in_room(coord_t point, SDL_Rect rect);
int                     is_corner_wall(SDL_Rect rect, int i, int j);
int                     is_room_wall(SDL_Rect room, int i, int j);
room_t                  *find_room_by_id(state_t *state, int id);
int                     room_is_valid(state_t *state, SDL_Rect room, int spacing, int min_size);
SDL_Rect                place_new_room(state_t *state, int max_rect_side);
coord_t                 room_center(SDL_Rect room);
SDL_Rect                g_rect(int grid_w, int grid_h, int w, int h);
void                    free_rooms(state_t *state);
void                    rooms_append(state_t *state, SDL_Rect room, int id);

#endif
