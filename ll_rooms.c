#include "includes/common.h"

void 		free_rooms(state_t *state) {
	room_t *current = state->rooms;
	room_t *next = NULL;

	while (current) {
		next = current->next;
		free(current);
		current = next;
	}

	state->rooms = NULL;
}

void 		rooms_append(state_t *state, SDL_Rect room) {
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
		state->rooms->room = room;
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
	last_room->next->room = room;
	last_room->center = room_center(room);
	// state->grid[last_room->center.x][last_room->center.y] = 255;
	last_room->next->next = NULL;
}

void 		through_list(state_t *state) {
	room_t *tmp = state->rooms;

	while (tmp) {
		printf("%d - %d - %d - %d => center: %d - %d\n", tmp->room.x, tmp->room.y, tmp->room.w, tmp->room.h, tmp->center.x, tmp->center.y);
		tmp = tmp->next;
	}
}
