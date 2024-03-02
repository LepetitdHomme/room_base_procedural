#include "includes/common.h"

void          free_doors(room_t *room) {
  door_t *current = room->doors;
  door_t *next = NULL;

  while (current) {
    next = current->next;
    free_corridors(current);
    free(current);
    current = next;
  }

  room->doors = NULL;
}

void 		      free_rooms(state_t *state) {
	room_t *current = state->rooms;
	room_t *next = NULL;

	while (current) {
		next = current->next;
    free_doors(current);
		free(current);
		current = next;
	}

	state->rooms = NULL;
}

void          door_coordinates(door_t *door, room_t *room1, room_t *room2) {
  int         x1 = room1->center.x;
  int         y1 = room1->center.y;
  int         x2 = room2->center.x;
  int         y2 = room2->center.y;
  const int   dx = abs(x2 - x1);
  const int   dy = abs(y2 - y1);
  const int   sx = x1 < x2 ? 1 : -1;
  const int   sy = y1 < y2 ? 1 : -1;
  int         err = (dx > dy ? dx : -dy) / 2;
  int         e2;
  int         current_x = x1;
  int         current_y = y1;

  while (1) {
    // Check if the current position is within the boundaries of room1
    if (is_room_wall(room1->room, current_x, current_y) == 0) {
      door->door_src_dir = door_dir(room1->room, current_x, current_y);
      door->coord_src.x = current_x;
      door->coord_src.y = current_y;
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

  // Reset variables for the next line
  current_x = x2;
  current_y = y2;
  err = (dx > dy ? dx : -dy) / 2;

  while (1) {
    // Check if the current position is within the boundaries of room2
    if (is_room_wall(room2->room, current_x, current_y) == 0) {
      door->door_dst_dir = door_dir(room2->room, current_x, current_y);
      door->coord_dst.x = current_x;
      door->coord_dst.y = current_y;
      break;
    }

    if (current_x == x1 && current_y == y1) {
      printf("not intersect found\n");
      // We've reached the end point, but no valid intersection found
      // You may want to handle this case based on your requirements
      break;
    }

    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      current_x -= sx;
    }
    if (e2 < dy) {
      err += dx;
      current_y -= sy;
    }
  }
}

/* TODO: check mallocs */
void          doors_append(state_t *state, room_t *src, room_t *dst) {
  door_t      *tmp = src->doors;

  if (tmp == NULL) {
    src->doors = (door_t *)malloc(sizeof(door_t));
    tmp = src->doors;
  } else {
    while (tmp->next) {
      tmp = tmp->next;
    }    
    tmp->next = (door_t *)malloc(sizeof(door_t));
    tmp = tmp->next;
  }

  tmp->room = dst;
  tmp->corridors = NULL;
  tmp->next = NULL;
  door_coordinates(tmp, src, dst);
  dig_corridor(tmp, src, dst);
}

void          rooms_append(state_t *state, SDL_Rect room, int id) {
	room_t       *tmp = state->rooms;

	if (tmp == NULL) {
		state->rooms = (room_t *)malloc(sizeof(room_t));
    if (state->rooms == NULL) {
      DEBUG_MSG("Error during rooms_append#malloc");
      exit(EXIT_FAILURE);
    }
    tmp = state->rooms;
	} else {
    while (tmp->next) {
      tmp = tmp->next;
    }
    tmp->next = (room_t *)malloc(sizeof(room_t));
    if (tmp->next == NULL) {
      DEBUG_MSG("Error during rooms_append#malloc");
      exit(EXIT_FAILURE);
    }
    tmp = tmp->next;
  }

	tmp->id = id;
	tmp->room = room;
	tmp->doors = NULL;
	tmp->center = room_center(room);
	tmp->next = NULL;
}
