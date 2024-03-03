#include "includes/common.h"

SDL_Color     pick_color(state_t *state, int i, int j) {
  SDL_Color color;

  switch (state->grid[i][j]) {
    case EMPTY://empty
      color.r = 0;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    case WALL_UP:
    case WALL_DOWN:
    case WALL_LEFT:
    case WALL_RIGHT:
      color.r = 127;
      color.g = 127;
      color.b = 127;
      color.a = 127;
      break;
    case FLOOR://floor
      color.r = 255;
      color.g = 255;
      color.b = 255;
      color.a = 255;
      break;
    case CORRIDOR:
      color.r = 0;
      color.g = 50;
      color.b = 255;
      color.a = 255;
      break;
    case DOOR_SRC://door
      color.r = 0;
      color.g = 255;
      color.b = 0;
      color.a = 255;
      break;
    case DOOR_DST:
      color.r = 255;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    default:
      color.r = 255;
      color.g = 255;
      color.b = 255;
      color.a = 255;
  }

  return color;
}

enum Dir      invert_dir(enum Dir dir) {
  enum Dir  invert;

  switch (dir) {
  case UP:
    invert = DOWN;
    break;
  case DOWN:
    invert = UP;
    break;
  case LEFT:
    invert = RIGHT;
    break;
  default:
    invert = LEFT;
  }

  return invert;
}

double        angle_from_type(enum Type type) {
  double angle;

  switch (type) {
    case EMPTY:
      angle = 0.0;
      break;
    case WALL_UP:
    case DOOR_UP:
      angle = 0.0;
      break;
    case WALL_DOWN:
    case DOOR_DOWN:
      angle = 180.0;
      break;
    case WALL_LEFT:
    case DOOR_LEFT:
      angle = -90.0;
      break;
    case WALL_RIGHT:
    case DOOR_RIGHT:
      angle = 90.0;
      break;
    case CORNER_TOP_LEFT:
      angle = 0.0;
      break;
    case CORNER_TOP_RIGHT:
      angle = 90.0;
      break;
    case CORNER_BOT_LEFT:
      angle = -90.0;
      break;
    case CORNER_BOT_RIGHT:
      angle = 180.0;// -180.0;
      break;
    default:
      angle = 0.0;
  }
  return angle;
}

enum Type     door_dir_to_type(enum Dir dir) {
  switch (dir) {
    case UP:
      return DOOR_UP;
      break;
    case DOWN:
      return DOOR_DOWN;
      break;
    case LEFT:
      return DOOR_LEFT;
      break;
    default:
      return DOOR_RIGHT;
  }
}

enum Type     wall_type(SDL_Rect room, int x, int y) {
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
