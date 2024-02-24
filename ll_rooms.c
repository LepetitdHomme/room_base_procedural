#include "includes/common.h"

void    free_doors(room_t *room) {
  door_t *current = room->doors;
  door_t *next = NULL;

  while (current) {
    next = current->next;
    current->room = NULL;
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

void    doors_append(state_t *state, room_t *src, room_t *dst) {
  if (src == NULL || dst == NULL) {
    return;
  }

  door_t *tmp = src->doors;

  if (tmp == NULL) {
    src->doors = (door_t *)malloc(sizeof(door_t));
    src->doors->room = dst;
    src->doors->next = NULL;
    return;
  }

  while (tmp->next) {
    tmp = tmp->next;
  }

  tmp->next = (door_t *)malloc(sizeof(door_t));
  tmp->next->room = dst;
  tmp->next->next = NULL;
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
