#include "includes/common.h"

void          free_corridors(door_t *door) {
  corridor_t *current = door->corridors;
  corridor_t *next = NULL;

  while (current) {
    next = current->next;
    free(current);
    current = next;
  }

  door->corridors = NULL;
}

void          next_coord_with_step(coord_t src, enum Dir dir, coord_t *dst) {
  dst->x = src.x;
  dst->y = src.y;

  switch (dir) {
    case UP:
      dst->y -= 1;
      break;
    case DOWN:
      dst->y += 1;
      break;
    case LEFT:
      dst->x -= 1;
      break;
    default:
      dst->x += 1;
  }
}

SDL_Rect      rect_from_doors(coord_t a, enum Dir dir_a, coord_t b, enum Dir dir_b) {
  SDL_Rect  rect;


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

void          corridors_append(door_t *door, room_t *room1, room_t *room2) {
  corridor_t  *corridor = door->corridors;
  coord_t     head,tail;

  if ((door->corridors = (corridor_t *)malloc(sizeof(corridor_t))) == NULL) {
    DEBUG_MSG("Malloc error");
    exit(EXIT_FAILURE);    
  }
  door->corridors->rect = rect_from_doors(door->coord_src, door->door_src_dir, door->coord_dst, door->door_dst_dir);
  door->corridors->next = NULL;
}
