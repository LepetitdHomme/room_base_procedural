#include "includes/common.h"

int       max_rect_side(int x, int y, int n) {
  // Calculate the maximum side length of rectangles
  double px = ceil(sqrt(n * x / y));
  double py = ceil(sqrt(n * y / x));
  double sx = (floor(px * y / x) * px < n) ? (y / ceil(px * y / x)) : (x / px);
  double sy = (floor(py * x / y) * py < n) ? (x / ceil(py * x / y)) : (y / py);
  double max_rect_side = MAX(sx, sy);
  return((int)max_rect_side + 1);
}

void      init_level(state_t *state, int complexity) {
  int rooms_number,max_rect;
  SDL_Rect room;

  free_level(state);
  init_grid(state, complexity);
  state->num_rooms = BASE_ROOM_NUMBER * complexity;
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
  init_player(state);
}

void     free_level(state_t *state) {
  if (state->grid != NULL) {
    free_grid(state);
  }
  if (state->rooms != NULL) {
    free_rooms(state);
  }
}

void    draw_connections(state_t *state) {
  room_t *tmp = state->rooms;
  door_t *tmp_d = NULL;

  while (tmp) {
    tmp_d = tmp->doors;
    while (tmp_d) {
      coord_t a,b;
      a.x = tmp->center.x * state->scale;
      a.y = tmp->center.y * state->scale;
      b.x = tmp_d->room->center.x * state->scale;
      b.y = tmp_d->room->center.y * state->scale;
      SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
      SDL_RenderDrawLine(state->renderer, a.x, a.y, b.x, b.y);
      tmp_d = tmp_d->next;
    }
    tmp = tmp->next;
  }
}

void    level_into_grid(state_t *state) {
  room_t *current = NULL;
  door_t *door = NULL;
  
  // printf("grid: %d - %d\n", state->grid_w, state->grid_h);
  // reset grid
  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      state->grid[i][j] = EMPTY;
    }
  }

  current = state->rooms;
  while (current != NULL) {
    // place rooms on grid
    // printf("room center: %d - %d\n", current->center.x, current->center.y);
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
      // printf("door src: %d - %d | door dst: %d - %d\n", door->coord_src.x, door->coord_src.y, door->coord_dst.x, door->coord_dst.y);
      state->grid[door->coord_src.x][door->coord_src.y] = DOOR_SRC;
      state->grid[door->coord_dst.x][door->coord_dst.y] = DOOR_DST;
      door = door->next;
    }

    // place corridors on grid
    current = current->next;
  }

  // for (int i = 0; i < state->grid_w ; i++) {
  //   for (int j = 0 ; j < state->grid_h ; j++) {
  //     printf("%d", state->grid[i][j]);
  //   }
  //   printf("\n");
  // }
}
