#include "includes/common.h"

SDL_Rect      g_rect(int grid_w, int grid_h, int w, int h) {
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

int           is_corner_wall(SDL_Rect room, int i, int j) {
  if (i == room.x && j == room.y || i == room.x + room.w && j == room.y || i == room.x && j == room.y + room.h || i == room.x + room.w && j == room.y + room.h) {
    return 0;
  }
  return 1;
}

int is_room_wall(SDL_Rect room, int x, int y) {
  // Check if (x, y) lies on any of the four edges of the rectangle
  if ((x == room.x || x == room.x + room.w - 1) && (y >= room.y && y < room.y + room.h) ||
    (y == room.y || y == room.y + room.h - 1) && (x >= room.x && x < room.x + room.w)) {
    return 0; // Coordinate is on the wall of the room rectangle
  } else {
    return 1; // Coordinate is not on the wall of the room rectangle
  }
}

enum Type wall_type(SDL_Rect room, int x, int y) {
  enum Type type;

  if (y == room.y) {
    if (x == room.x)
      return CORNER_TOP_LEFT;
    if (x == room.x + room.w - 1)
      return CORNER_TOP_RIGHT;
    return WALL_UP;
  }
  if (y == room.y + room.h - 1) {
    if (x == room.x)
      return CORNER_BOT_LEFT;
    if (x == room.x + room.w - 1)
      return CORNER_BOT_RIGHT;
    return WALL_DOWN;
  }
  if (x == room.x)
    return WALL_LEFT;
  if (x == room.x + room.w - 1)
    return WALL_RIGHT;
  return EMPTY;
}


coord_t       room_center(SDL_Rect room) {
  coord_t center;

  center.x = room.w / 2 + room.x;
  center.y = room.h / 2 + room.y;

  return center;
}

SDL_Rect        place_new_room(state_t *state, int max_rect_side) {
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
        rect.x = -1; // custom failure
        return rect;
      }
    }
  }



  // TODO: draw later on the grid ; from ll_rooms.
  // int color = 127;

  // place random int != 0 in grid during rectangle/rooms generation
  for(int i = rect.x ; i < rect.x + rect.w ; i++) {
    for (int j = rect.y ; j < rect.y + rect.h ; j++) {
      state->grid[i][j] = 1;
    }
  }
  return rect;
}
