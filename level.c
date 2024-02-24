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

  rooms_number = state->num_rooms;

  while (rooms_number > 0) {
    room = place_new_room(state, max_rect);
    if (room.x == -1) {
      continue;
    }
    rooms_number--;

    // add room to state level rooms
    rooms_append(state, room);
  }
}

void     free_level(state_t *state) {
  if (state->grid != NULL) {
    free_grid(state);
  }
  if (state->rooms != NULL) {
    free_rooms(state);
  }
}