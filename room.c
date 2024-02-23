#include "includes/common.h"

SDL_Rect    g_rect(int grid_w, int grid_h, int w, int h) {
  SDL_Rect rect;

  int width, height;
  if (w == -1 || h == -1) {
    width = random_int(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
    height = random_int(MIN_ROOM_SIZE, MAX_ROOM_SIZE);
  } else {
    width = w;
    height = h;
  }

  // printf("room: %d - %d\n", width, height);
  rect.x = random_int(1, grid_w - 1 - width);
  rect.y = random_int(1, grid_h - 1 - height);
  rect.w = width;
  rect.h = height;

  return rect;
}

int        place_new_room(state_t *state, int max_rect_side) {
  if (max_rect_side <= 5) {
    printf("error when placing room\n");
    exit(EXIT_FAILURE);
  }
  SDL_Rect rect;

  rect = g_rect(state->grid_w, state->grid_h, random_int(5, max_rect_side), random_int(5, max_rect_side));
  // printf("%d - %d - %d - %d\n", rect.x, rect.y, rect.w, rect.h);

  for (int i = rect.x - 1 ; i < rect.x + rect.w + 1 ; i++) {
    for (int j = rect.y - 1; j < rect.y + rect.h + 1 ; j++) {
      if (state->grid[i][j] != 0) {
        return 1;
      }
    }
  }

  int color = 255;

  for(int i = rect.x ; i < rect.x + rect.w ; i++) {
    for (int j = rect.y ; j < rect.y + rect.h ; j++) {
      state->grid[i][j] = color;
    }
  }
  return 0;
}
