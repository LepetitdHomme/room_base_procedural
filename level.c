#include "includes/common.h"

int           max_rect_side(int x, int y, int n);

void          init_level(state_t *state, int complexity) {
  int         rooms_number,max_rect,clamped_complexity;
  SDL_Rect    room;

  free_level(state);
  clamped_complexity = clamp(complexity, MIN_COMPLEXITY, MAX_COMPLEXITY);
  init_grid(state, clamped_complexity);
  state->num_rooms = BASE_ROOM_NUMBER * clamped_complexity;
  max_rect = max_rect_side(state->grid_w, state->grid_h, state->num_rooms);

  rooms_number = 0;

  while (rooms_number < state->num_rooms) {
    room = place_new_room(state, max_rect);
    if (room.x == -1) {
      continue;
    }
    // add room to state level rooms
    rooms_append(state, room, rooms_number);
    rooms_number++;

  }
  apply_kruskal(state);
  // init_player(state); // => to free
}

int           max_rect_side(int x, int y, int n) {
  double      px = ceil(sqrt(n * x / y));
  double      py = ceil(sqrt(n * y / x));
  double      sx = (floor(px * y / x) * px < n) ? (y / ceil(px * y / x)) : (x / px);
  double      sy = (floor(py * x / y) * py < n) ? (x / ceil(py * x / y)) : (y / py);
  double      max_rect_side = MAX(sx, sy);

  return((int)max_rect_side + 1);
}

void          free_level(state_t *state) {
  if (state->grid != NULL) {
    free_grid(state);
  }
  if (state->rooms != NULL) {
    free_rooms(state);
  }
}

void          level_into_grid(state_t *state) {
  room_t      *current = NULL;
  door_t      *door = NULL;
  corridor_t  *corridor = NULL;
  
  // reset grid
  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      state->grid[i][j] = EMPTY;
    }
  }

  current = state->rooms;
  while (current != NULL) {
    // place rooms on grid
    for (int i = current->room.x ; i < current->room.x + current->room.w ; i++) {
      for (int j = current->room.y ; j < current->room.y + current->room.h ; j++) {
        if (state->grid[i][j] != 0)
          continue;
        if (is_room_wall(current->room, i, j) == 0) {
          state->grid[i][j] = wall_type(current->room, i, j);
        } else {
          state->grid[i][j] = FLOOR; // floor
        }
      }
    }

    // place doors on grid
    door = current->doors;
    while (door != NULL) {
      state->grid[door->coord_src.x][door->coord_src.y] = DOOR_SRC;
      state->grid[door->coord_dst.x][door->coord_dst.y] = DOOR_DST;

      // place corridors on grid
      corridor = door->corridors;
      while (corridor != NULL) {
        for (int i = corridor->rect.x ; i < corridor->rect.x + corridor->rect.w ; i++) {
          for (int j = corridor->rect.y ; j < corridor->rect.y + corridor->rect.h ; j++) {
            state->grid[i][j] = CORRIDOR;
          }
        }
        corridor = corridor->next;
      }
      door = door->next;
    }

    // place corridors on grid
    current = current->next;
  }
}
