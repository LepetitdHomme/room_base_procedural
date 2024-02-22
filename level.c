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
  int rooms_number,tmp,max_rect;

  init_grid(state, complexity);
  rooms_number = BASE_ROOM_NUMBER * complexity;
  max_rect = max_rect_side(state->grid_w, state->grid_h, rooms_number);

  while (rooms_number > 0) {
    tmp = place_new_room(state, max_rect);
    if (tmp == 0)
      rooms_number--;
  }
}
