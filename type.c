#include "includes/common.h"

SDL_Color     type_to_map_color(enum Type type) {
  SDL_Color color;

  switch (type) {
    case EMPTY:
      color.r = 0;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    case FLOOR:
    case CORRIDOR:
      color.r = 100;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    case WALL_UP:
    case WALL_DOWN:
    case WALL_LEFT:
    case WALL_RIGHT:
    case CORNER_TOP_LEFT:
    case CORNER_TOP_RIGHT:
    case CORNER_BOT_LEFT:
    case CORNER_BOT_RIGHT:
      color.r = 150;
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
      color.r = 100;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    case CORRIDOR: // should not be used anymore => corridors become floor(room)
      color.r = 0;
      color.g = 50;
      color.b = 255;
      color.a = 255;
      break;
    case DOOR_UP:
    case DOOR_DOWN:
    case DOOR_LEFT:
    case DOOR_RIGHT:
      color.r = 0;
      color.g = 255;
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

SDL_RendererFlip   flip_from_type(enum Type type) {
  SDL_RendererFlip    flip;

  switch (type) {
    case CORNER_TOP_RIGHT:
    case WALL_RIGHT:
    case CORNER_BOT_RIGHT:
    case DOOR_RIGHT:
      flip = SDL_FLIP_HORIZONTAL;
      break;
    default:
      flip = SDL_FLIP_NONE;
  }

  return flip;
}

double        angle_from_type(enum Type type) {
  double angle;

  switch (type) {
    case EMPTY:
      angle = 0.0;
      break;
    // case WALL_UP:
    // case DOOR_UP:
    //   angle = 0.0;
    //   break;
    // case WALL_DOWN:
    // case DOOR_DOWN:
    //   angle = 180.0;
    //   break;
    // case WALL_LEFT:
    // case DOOR_LEFT:
    //   angle = -90.0;
    //   break;
    // case WALL_RIGHT:
    // case DOOR_RIGHT:
    //   angle = 90.0;
    //   break;
    // case CORNER_TOP_LEFT:
    //   angle = 0.0;
    //   break;
    // case CORNER_TOP_RIGHT:
    //   angle = 90.0;
    //   break;
    // case CORNER_BOT_LEFT:
    //   angle = -90.0;
    //   break;
    // case CORNER_BOT_RIGHT:
    //   angle = 180.0;// -180.0;
    //   break;
    default:
      angle = 0.0;
  }
  return angle;
}

int           is_door_type(enum Type type) {
  if (type == DOOR_UP || type == DOOR_DOWN || type == DOOR_LEFT || type == DOOR_RIGHT || type == DOOR_UP_OPEN || type == DOOR_DOWN_OPEN) {
    return 0;
  }

  return 1;
}

int           is_floor_type(enum Type type) {
  if (type == FLOOR || type == CORRIDOR) {
    return 0;
  }

  return 1;
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
