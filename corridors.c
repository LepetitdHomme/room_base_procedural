#include "includes/common.h"

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

/*            performs inversion of x,y imposed by SDL DRAW */
/* 1709163157 */
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

void          dig_corridor(door_t *door, room_t *room1, room_t *room2) {
  // orientation = (door->door_src_dir == UP || door->door_src_dir == DOWN) ? VERTICAL : HORIZONTAL;
  corridor_t  *corridor = door->corridors;
  coord_t     head,tail;

  if ((door->corridors = (corridor_t *)malloc(sizeof(corridor_t))) == NULL) {
    DEBUG_MSG("Malloc error");
    exit(EXIT_FAILURE);    
  }
  // next_coord_with_step(door->coord_src, door->door_src_dir, &head);
  // next_coord_with_step(door->coord_dst, door->door_dst_dir, &tail);
  door->corridors->rect = rect_from_doors(door->coord_src, door->door_src_dir, door->coord_dst, door->door_dst_dir);
  door->corridors->next = NULL;
}

// void          door_to_door(door_t *door, room_t *room1, room_t *room2) {
//   coord_t           head,tail;
//   enum Orientation  orientation;
//   int               dist_x, dist_y;
//   corridor_t        *current_corridor;

//   door->corridors = (corridor_t *)malloc(sizeof(corridor_t));
//   if (door->corridors == NULL) {
//     DEBUG_MSG("Malloc error");
//     exit(EXIT_FAILURE);
//   }
//   door->corridors->next = NULL;

//   orientation = (door->door_src_dir == UP || door->door_src_dir == DOWN) ? VERTICAL : HORIZONTAL;
//   next_coord_with_step(door->coord_src, door->door_src_dir, &head);
//   next_coord_with_step(door->coord_dst, door->door_dst_dir, &tail);
//   door->corridors->floor = head;
//   next_coord_with_step(head, (orientation == VERTICAL) ? LEFT : UP, &door->corridors->wall_left);
//   door->corridors->wall_left_type = (orientation == VERTICAL) ? WALL_LEFT : WALL_UP;
//   next_coord_with_step(head, (orientation == VERTICAL) ? RIGHT : DOWN, &door->corridors->wall_right);
//   door->corridors->wall_right_type = (orientation == VERTICAL) ? WALL_RIGHT : WALL_DOWN;

//   while (head.x != tail.x || head.y != tail.y) {
//     dist_x = abs(tail.x - head.x);
//     dist_y = abs(tail.x - head.y);

//     if (dist_x > dist_y) {
//       if (head.x < tail.x) {
//         head.x += 1;
//       } else if (head.x > tail.x) {
//         head.x -= 1;
//       }
//     } else {
//       if (head.y < tail.y) {
//         head.y += 1;
//       } else if (head.y > tail.y) {
//         head.y -= 1;
//       }
//     }

//     // if (is_room_wall(room2->room, head.x, head.y) == 0) {
//     //   door->coord_dst.x = head.x;
//     //   door->coord_dst.y = head.y;
//     //   break;
//     // }
//     // if (is_room_wall(room1->room, head.x, head.y) == 0) {
//     //   break;
//     // }
//   } 

//   door->corridors->next = (corridor_t *)malloc(sizeof(corridor_t));
//   if (door->corridors->next == NULL) {
//     DEBUG_MSG("Malloc error");
//     exit(EXIT_FAILURE);
//   }
//   door->corridors->next->floor = tail;
//   orientation = (door->door_dst_dir == UP || door->door_dst_dir == DOWN) ? VERTICAL : HORIZONTAL;
//   next_coord_with_step(tail, (orientation == VERTICAL) ? LEFT : UP, &door->corridors->next->wall_left);
//   door->corridors->next->wall_left_type = (orientation == VERTICAL) ? WALL_LEFT : WALL_UP;
  
//   next_coord_with_step(tail, (orientation == VERTICAL) ? RIGHT : DOWN, &door->corridors->next->wall_right);
//   door->corridors->next->wall_right_type = (orientation == VERTICAL) ? WALL_RIGHT : WALL_DOWN;
  
//   door->corridors->next->next = NULL;
// }