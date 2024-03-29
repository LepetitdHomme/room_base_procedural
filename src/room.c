#include <stdlib.h>
#include "../include/macros.h"
#include "../include/room.h"
#include "../include/tools.h"

SDL_Rect      g_rect(int grid_w, int grid_h, int w, int h) {
  SDL_Rect    rect;

  rect.x = random_int(DISTANCE_BETWEEN_ROOMS, grid_w - DISTANCE_BETWEEN_ROOMS - w);
  rect.y = random_int(DISTANCE_BETWEEN_ROOMS, grid_h - DISTANCE_BETWEEN_ROOMS - h);
  rect.w = w;
  rect.h = h;

  return rect;
}

room_t        *find_room_by_id(state_t *state, int id) {
  room_t *tmp = state->rooms;

  while (tmp) {
    if (tmp->id == id) {
      return tmp;
    }
    tmp = tmp->next;
  }
  return NULL;
}

int           is_corner_wall(SDL_Rect rect, int i, int j) {
  if (i == rect.x && j == rect.y) {
    return 0;
  }
  if (i == rect.x + rect.w - 1 && j == rect.y) {
    return 0;
  }
  if (i == rect.x && j == rect.y + rect.h - 1) {
    return 0;
  }
  if (i == rect.x + rect.w - 1 && j == rect.y + rect.h - 1) {
    return 0;
  }

  return 1;
}

/*            Checks if (x, y) lies on any of the four edges of the rectangle */
int           is_room_wall(SDL_Rect room, int x, int y) {
  if (x == room.x || x == room.x + room.w - 1) {
    if (y >= room.y && y < room.y + room.h) {
      return 0;
    }
  }
  if (y == room.y || y == room.y + room.h - 1) {
    if (x >= room.x && x < room.x + room.w) {
      return 0;
    }
  }

  return 1;
}

int           is_in_room(coord_t point, SDL_Rect rect) {
  if (point.x >= rect.x && point.x < rect.x + rect.w && point.y >= rect.y && point.y < rect.y + rect.h) {
    return 0;
  }

  return 1;
}

coord_t       room_center(SDL_Rect room) {
  coord_t center;

  center.x = room.w / 2 + room.x;
  center.y = room.h / 2 + room.y;

  return center;
}

int           room_is_valid(state_t *state, SDL_Rect room, int spacing, int min_size) {
  if (room.w < min_size || room.h < min_size) {
    return 2;
  }

  for (int i = room.x - spacing ; i < room.x + room.w + spacing ; i++) {
    for (int j = room.y - spacing; j < room.y + room.h + spacing ; j++) {
      if (state->grid[i][j] != 0) {
        return 1;
      }
    }
  }

  return 0;
}

void          tmp_fill_grid(state_t *state, SDL_Rect rect) {
  // place random int != 0 in grid during rectangle/rooms generation
  // UNTIL CORRIDORS DONE
  for(int i = rect.x ; i < rect.x + rect.w ; i++) {
    for (int j = rect.y ; j < rect.y + rect.h ; j++) {
      state->grid[i][j] = 1;
    }
  }
}

SDL_Rect      place_new_room(state_t *state, int max_rect_side) {
  if (max_rect_side <= 5) {
    printf("error when placing room\n");
    exit(EXIT_FAILURE);
  }
  SDL_Rect rect;

  rect = g_rect(state->grid_w, state->grid_h, random_int(5, max_rect_side), random_int(5, max_rect_side));
  // printf("%d - %d - %d - %d\n", rect.x, rect.y, rect.w, rect.h);

  if (room_is_valid(state, rect, DISTANCE_BETWEEN_ROOMS, MIN_ROOM_SIZE) != 0) {
    rect.x = -1; // custom failure
    return rect;
  }

  tmp_fill_grid(state, rect);
  return rect;
}

void          free_rooms(state_t *state) {
  room_t *current = state->rooms;
  room_t *next = NULL;

  while (current) {
    next = current->next;
    free(current);
    current = next;
  }

  state->rooms = NULL;
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
  tmp->center = room_center(room);
  tmp->next = NULL;
}

