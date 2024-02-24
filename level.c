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
  // Kruskal !
  apply_kruskal(state);
  // Minimum Spanning Tree
  // min_spanning_tree(state);
}

void     free_level(state_t *state) {
  if (state->grid != NULL) {
    free_grid(state);
  }
  if (state->rooms != NULL) {
    free_rooms(state);
  }
}

void    draw_level(state_t *state) {
  room_t *current = state->rooms;
  door_t *tmp_door = NULL;
  SDL_Rect rect;

  // for (int i = 0; i < state->grid_w ; i++) {
  //   for (int j = 0 ; j < state->grid_h ; j++) {
  //     rect.x = i * state->scale;
  //     rect.y = j * state->scale;
  //     rect.w = state->scale;
  //     rect.h = state->scale;

  //     SDL_SetRenderDrawColor(state->renderer, state->grid[i][j], state->grid[i][j], state->grid[i][j], 255);
  //     SDL_RenderFillRect(state->renderer, &rect);
  //   }
  // }

  coord_t center1,center2;
  SDL_Rect room;
  
  while (current != NULL) {
    // draw room
    room.x = current->room.x * state->scale;
    room.y = current->room.y * state->scale;
    room.w = current->room.w * state->scale;
    room.h = current->room.h * state->scale;
    if (current->id == 0) {
      SDL_SetRenderDrawColor(state->renderer, 0, 255, 0, 255);
    } else if (current->id == state->num_rooms - 1) {
      SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
    } else {
      SDL_SetRenderDrawColor(state->renderer, 255, 255, 0, 255);
    }
    SDL_RenderDrawRect(state->renderer, &room);

    // draw centers and closests
    center1.x = current->center.x * state->scale;
    center1.y = current->center.y * state->scale;
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    rect.x = center1.x;
    rect.y = center1.y;
    rect.w = state->scale;
    rect.h = state->scale;
    SDL_RenderDrawRect(state->renderer, &rect);

    if (current->doors != NULL) {
      tmp_door = current->doors;
      while (tmp_door != NULL) {
        center2.x = tmp_door->room->center.x * state->scale;
        center2.y = tmp_door->room->center.y * state->scale;
        SDL_SetRenderDrawColor(state->renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(state->renderer, center1.x, center1.y, center2.x, center2.y);
        tmp_door = tmp_door->next;
      }
    }
    current = current->next;
  }
}
