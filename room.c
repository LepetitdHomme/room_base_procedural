#include "includes/common.h"

SDL_Rect      g_rect(int grid_w, int grid_h, int w, int h) {
  SDL_Rect    rect;

  rect.x = random_int(DISTANCE_BETWEEN_ROOMS, grid_w - DISTANCE_BETWEEN_ROOMS - w);
  rect.y = random_int(DISTANCE_BETWEEN_ROOMS, grid_h - DISTANCE_BETWEEN_ROOMS - h);
  rect.w = w;
  rect.h = h;

  return rect;
}

int           is_corner_wall(SDL_Rect room, int i, int j) {
  if (i == room.x && j == room.y) {
    return 0;
  }
  if (i == room.x + room.w && j == room.y) {
    return 0;
  }
  if (i == room.x && j == room.y + room.h) {
    return 0;
  }
  if (i == room.x + room.w && j == room.y + room.h) {
    return 0;
  }

  return 1;
}

/*            Checks if (x, y) lies on any of the four edges of the rectangle */
int           is_room_wall(SDL_Rect room, int x, int y) {
  if (x == room.x || x == room.x + room.w - 1) {
    if (y >= room.y && y < room.y + room.h) {
      return 0;
    }
  }
  if (y == room.y || y == room.y + room.h - 1) {
    if (x >= room.x && x < room.x + room.w) {
      return 0;
    }
  }

  return 1;
}

enum Dir      door_dir(SDL_Rect room, int x, int y) {
  enum Dir dir;

  if (y == room.y) {
    return UP;
  }
  if (y == room.y + room.h - 1) {
    return DOWN;
  }
  if (x == room.x)
    return LEFT;
  if (x == room.x + room.w - 1)
    return RIGHT;
  return UP;
}

coord_t       room_center(SDL_Rect room) {
  coord_t center;

  center.x = room.w / 2 + room.x;
  center.y = room.h / 2 + room.y;

  return center;
}

SDL_Rect      place_new_room(state_t *state, int max_rect_side) {
  if (max_rect_side <= 5) {
    printf("error when placing room\n");
    exit(EXIT_FAILURE);
  }
  SDL_Rect rect;

  rect = g_rect(state->grid_w, state->grid_h, random_int(5, max_rect_side), random_int(5, max_rect_side));
  // rect = g_rect(state->grid_w, state->grid_h, max_rect_side, (max_rect_side * RATIO_HEIGHT) / RATIO_WIDTH);
  // printf("%d - %d - %d - %d\n", rect.x, rect.y, rect.w, rect.h);

  for (int i = rect.x - DISTANCE_BETWEEN_ROOMS ; i < rect.x + rect.w + DISTANCE_BETWEEN_ROOMS ; i++) {
    for (int j = rect.y - DISTANCE_BETWEEN_ROOMS; j < rect.y + rect.h + DISTANCE_BETWEEN_ROOMS ; j++) {
      if (state->grid[i][j] != 0) {
        rect.x = -1; // custom failure
        return rect;
      }
    }
  }

  // place random int != 0 in grid during rectangle/rooms generation
  for(int i = rect.x ; i < rect.x + rect.w ; i++) {
    for (int j = rect.y ; j < rect.y + rect.h ; j++) {
      state->grid[i][j] = 1;
    }
  }
  return rect;
}
