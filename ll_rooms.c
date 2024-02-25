#include "includes/common.h"

void    free_doors(room_t *room) {
  door_t *current = room->doors;
  door_t *next = NULL;

  DEBUG_MSG("");
  while (current) {
    next = current->next;
    free(current);
    current = next;
  }

  room->doors = NULL;
}

void 		free_rooms(state_t *state) {
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

// Function to calculate the equation of a line given two points
// void calculate_line_equation(coord_t p1, coord_t p2, double *m, double *c) {
//   // Calculate slope
//   *m = (double)(p2.y - p1.y) / (double)(p2.x - p1.x);

//   // Calculate y-intercept
//   *c = p1.y - (*m) * p1.x;
// }

#include <stdlib.h>

void determine_door_coordinates(door_t *door, room_t *room1, room_t *room2) {
  int x1 = room1->center.x;
  int y1 = room1->center.y;
  int x2 = room2->center.x;
  int y2 = room2->center.y;

  const int dx = abs(x2 - x1);
  const int dy = abs(y2 - y1);
  const int sx = x1 < x2 ? 1 : -1;
  const int sy = y1 < y2 ? 1 : -1;

  int err = (dx > dy ? dx : -dy) / 2;
  int e2;

  int current_x = x1;
  int current_y = y1;

  while (1) {
    // Check if the current position is within the boundaries of room1
    if (is_room_wall(room1->room, current_x, current_y) == 0) {
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

void    doors_append(state_t *state, room_t *src, room_t *dst) {
  if (src == NULL || dst == NULL) {
    return;
  }
  DEBUG_MSG("");
  door_t *tmp = src->doors;

  if (tmp == NULL) {
    src->doors = (door_t *)malloc(sizeof(door_t));
    src->doors->room = dst;
    src->doors->next = NULL;
    determine_door_coordinates(src->doors, src, dst);
    return;
  }

  while (tmp->next) {
    tmp = tmp->next;
  }

  tmp->next = (door_t *)malloc(sizeof(door_t));
  tmp->next->room = dst;
  tmp->next->next = NULL;
  determine_door_coordinates(tmp->next, src, dst);
}

void 		rooms_append(state_t *state, SDL_Rect room, int id) {
	room_t *current = state->rooms;
	room_t *last_room = NULL;

	while (current != NULL) {
		last_room = current;
		current = current->next;
	}

	if (last_room == NULL) {
		state->rooms = (room_t *)malloc(sizeof(room_t));
		if (state->rooms == NULL) {
			DEBUG_MSG("Error during rooms_append#malloc");
			exit(EXIT_FAILURE);
		}
		state->rooms->id = id;
		state->rooms->room = room;
		state->rooms->doors = NULL;
		state->rooms->center = room_center(room);
		// state->grid[state->rooms->center.x][state->rooms->center.y] = 255;
		state->rooms->next = NULL;
		return;
	}

	last_room->next = (room_t *)malloc(sizeof(room_t));
	if (last_room->next == NULL) {
		DEBUG_MSG("Error during rooms_append#malloc");
		exit(EXIT_FAILURE);
	}
	last_room->next->id = id;
	last_room->next->room = room;
	last_room->next->doors = NULL;
	last_room->next->center = room_center(room);
	// state->grid[last_room->next->center.x][last_room->next->center.y] = 255;
	last_room->next->next = NULL;
}

void 		through_list(state_t *state) {
	room_t *tmp = state->rooms;
  door_t *tmp2 = NULL;

	while (tmp) {
		// printf("%d - %d - %d - %d => center: %d - %d\n", tmp->room.x, tmp->room.y, tmp->room.w, tmp->room.h, tmp->center.x, tmp->center.y);
		// if (tmp->doors == NULL) {
		// 	printf("doors NULL\n");
		// } else {
    //   tmp2->doors
    //   while 
		// 	printf("link: %d => %d\n", tmp->id, tmp->closest->id);
		// }
		tmp = tmp->next;
	}
}
