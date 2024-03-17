#include "../include/type.h"
#include "../include/macros.h"

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
    case DOOR_UP:
    case DOOR_UP_OPEN:
    case DOOR_DOWN:
    case DOOR_DOWN_OPEN:
    case DOOR_LEFT:
    case DOOR_RIGHT:
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
      color.r = 200;
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

int           direction_to_degrees(enum Dir dir) {
  int         angle;

  switch (dir) {
    case UP:
      angle = -90;
      break;
    case DOWN:
      angle = -270;
      break;
    case RIGHT:
      angle = 0;
      break;
    case LEFT:
      angle = -180;
      break;
    default:
      angle = 0;
  }

  return angle;
}

/*
*   In which direction is the wall we are looking at ? player to grid cell direction
*/
enum Octant wall_to_octant(coord_t src, coord_t dst) {
  int dx = dst.x - src.x;
  int dy = dst.y - src.y;

  // Check relative position to determine octant
  if (dx == 0) {
    if (dy > 0) {
      return S;
    } else {
      return N;
    }
  } else if (dy == 0) {
    if (dx > 0) {
      return E;
    } else {
      return W;
    }
  } else {
    if (dx > 0) {
      if (dy > 0) {
        return SE;
      } else {
        return NE;
      }
    } else {
      if (dy > 0) {
        return SW;
      } else {
        return NW;
      }
    }
  }
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

int           is_wall_type(enum Type type) {
  if (type == WALL_RIGHT || type == WALL_UP || type == WALL_DOWN || type == WALL_LEFT) {
    return 0;
  }

  return 1;
}

int           is_corner_type(enum Type type) {
  if (type == CORNER_TOP_RIGHT || type == CORNER_TOP_LEFT || type == CORNER_BOT_LEFT || type ==  CORNER_BOT_RIGHT) {
    return 0;
  }

  return 1;
}

int           is_wall_or_corner_type(enum Type type) {
  if (is_wall_type(type) == 0 || is_corner_type(type) == 0) {
    return 0;
  }

  return 1;
}

int           type_stops_light(enum Type type) {
  if (type == CORRIDOR) {
    return 0;
  }
  return 1;
  // int res;

  // switch (type) {
  //   case FLOOR:
  //   case CORRIDOR:
  //   case DOOR_UP_OPEN:
  //   case DOOR_DOWN_OPEN:
  //     res = 1;
  //     break;
  //   default:
  //     res = 0;
  // }

  // return res;
}

enum Octant   corner_to_octant(enum Type corner) {
  enum Octant octant;

  switch (corner) {
    case CORNER_TOP_RIGHT:
      octant = NE;
      break;
    case CORNER_TOP_LEFT:
      octant = NW;
      break;
    case CORNER_BOT_LEFT:
      octant = SW;
      break;
    default:
      octant = SE;
  }

  return octant;
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
