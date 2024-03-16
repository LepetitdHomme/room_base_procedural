#include <stdlib.h>
#include "../include/macros.h"
#include "../include/door.h"
#include "../include/room.h"

void          doors_append(graph_t *src_node, door_t door_node) {
  src_node->num_doors += 1;

  src_node->doors = (door_t *)realloc(src_node->doors, src_node->num_doors * sizeof(door_t));
  if (src_node->doors == NULL) {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  src_node->doors[src_node->num_doors - 1] = door_node;
}

void          open_door(state_t *state) {
  enum Type type;
  coord_t   next;
  SDL_Rect  room_grid;

  room_grid.x = 0;
  room_grid.y = 0;
  room_grid.w = state->grid_w;
  room_grid.h = state->grid_h;

  type = state->grid[state->player->pos.x][state->player->pos.y];
  if (type == DOOR_UP) {
    state->grid[state->player->pos.x][state->player->pos.y] = DOOR_UP_OPEN;
    next.x = state->player->pos.x;
    next.y = state->player->pos.y - 1;
    if (is_in_room(next, room_grid) == 0 && state->grid[next.x][next.y] == DOOR_DOWN) {
      state->grid[next.x][next.y] = DOOR_DOWN_OPEN;
    }
  } else if (type == DOOR_DOWN) {
    state->grid[state->player->pos.x][state->player->pos.y] = DOOR_DOWN_OPEN;
    next.x = state->player->pos.x;
    next.y = state->player->pos.y + 1;
    if (is_in_room(next, room_grid) == 0 && state->grid[next.x][next.y] == DOOR_UP) {
      state->grid[next.x][next.y] = DOOR_UP_OPEN;
    }
  }
}

/* returns door for src */
door_t        door_coordinates(graph_t *src, graph_t *dst) {
  int         x1 = src->center.x;
  int         y1 = src->center.y;
  int         x2 = dst->center.x;
  int         y2 = dst->center.y;
  const int   dx = abs(x2 - x1);
  const int   dy = abs(y2 - y1);
  const int   sx = x1 < x2 ? 1 : -1;
  const int   sy = y1 < y2 ? 1 : -1;
  int         err = (dx > dy ? dx : -dy) / 2;
  int         e2;
  int         current_x = x1;
  int         current_y = y1;
  door_t      door_node;

  while (1) {
    // Check if the current position is within the boundaries of src
    if (is_room_wall(src->rect, current_x, current_y) == 0) {

      if (is_corner_wall(src->rect, current_x, current_y) == 0) {
        // printf("corner door ! => e2: %d, err: %d, sx: %d, sy: %d, dx: %d, dy: %d\n", e2, err, sx, sy, dx, dy);
        if (dx > dy) {
          if (sy > 0) {
            door_node.coord.x = current_x;
            door_node.coord.y = current_y - 1;
          } else {
            door_node.coord.x = current_x;
            door_node.coord.y = current_y + 1;
          }
        } else {
          if (sx > 0) {
            door_node.coord.x = current_x - 1;
            door_node.coord.y = current_y;
          } else {
            door_node.coord.x = current_x + 1;
            door_node.coord.y = current_y;
          }
        }
      } else {
        door_node.coord.x = current_x;
        door_node.coord.y = current_y;
      }
      door_node.dir = door_dir(src->rect, door_node.coord.x, door_node.coord.y);
      break;
    }

    if (current_x == x2 && current_y == y2) {
      printf("not intersect found\n");
      // We've reached the end point, but no valid intersection found
      // You may want to handle this case based on your requirements
      break;
    }

    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      current_x += sx;
    }
    if (e2 < dy) {
      err += dx;
      current_y += sy;
    }
  }
  door_node.dst_node = NULL;
  return door_node;
}

void          free_doors(graph_t *node) {
  free(node->doors);
  node->num_doors = 0;
}

enum Dir      door_dir(SDL_Rect rect, int x, int y) {
  if (y == rect.y) {
    return UP;
  }
  if (y == rect.y + rect.h - 1) {
    return DOWN;
  }
  if (x == rect.x)
    return LEFT;
  if (x == rect.x + rect.w - 1)
    return RIGHT;
  return UP;
}

coord_t       next_coord_with_step(coord_t src, enum Dir dir) {
  coord_t coord;
  coord.x = src.x;
  coord.y = src.y;

  switch (dir) {
    case UP:
      coord.y -= 1;
      break;
    case DOWN:
      coord.y += 1;
      break;
    case LEFT:
      coord.x -= 1;
      break;
    default:
      coord.x += 1;
  }

  return coord;
}

SDL_Rect      rect_from_doors(door_t src, door_t dst) {
  SDL_Rect  rect;
  coord_t   a = src.coord;
  coord_t   b = dst.coord;
  enum Dir  dir_a = src.dir;
  enum Dir  dir_b = dst.dir;

  if (b.x > a.x) {
    if (b.y > a.y) {
      // dir down right
      if (dir_a == RIGHT) {
        a.x += 1;
        a.y -= 1;
      } else { // DOWN
        a.x -= 1;
        a.y += 1;
      }
      if (dir_b == LEFT) {
        b.x -= 1;
        b.y += 1;
      } else { // TOP
        b.x += 1;
        b.y -= 1;
      }
      rect.w = b.x - a.x + 1;
      rect.h = b.y - a.y + 1;
      rect.x = a.x;
      rect.y = a.y;
    } else {
      // dir up right
      if (dir_a == RIGHT) {
        a.x += 1;
        a.y += 1;
      } else { // UP
        a.x -= 1;
        a.y -= 1;
      }
      if (dir_b == LEFT) {
        b.x -= 1;
        b.y -= 1;
      } else { // DOWN
        b.x += 1;
        b.y += 1;
      }
      rect.w = b.x - a.x + 1;
      rect.h = a.y - b.y + 1;
      rect.x = a.x;
      rect.y = b.y;
    }
  } else {
    if (a.y > b.y) {
      // dir up left
      if (dir_a == UP) {
        a.x += 1;
        a.y -= 1;
      } else { // LEFT
        a.x -= 1;
        a.y += 1;
      }
      if (dir_b == DOWN) {
        b.x -= 1;
        b.y += 1;
      } else { // RIGHT
        b.x += 1;
        b.y -= 1;
      }
      rect.w = a.x - b.x + 1;
      rect.h = a.y - b.y + 1;
      rect.x = b.x;
      rect.y = b.y;
    } else {
      // dir down left
      if (dir_a == DOWN) {
        a.x += 1;
        a.y += 1;
      } else { // LEFT
        a.x -= 1;
        a.y -= 1;
      }
      if (dir_b == UP) {
        b.x -= 1;
        b.y -= 1;
      } else { // RIGHT
        b.x += 1;
        b.y += 1;
      }
      rect.w = a.x - b.x + 1;
      rect.h = b.y - a.y + 1;
      rect.x = b.x;
      rect.y = a.y;
    }
  }

  return rect;
}

