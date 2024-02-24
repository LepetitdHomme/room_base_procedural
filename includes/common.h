#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define DEBUG_MSG(message) printf("Debug: %s, File %s, Line %d\n", message, __FILE__, __LINE__)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 10
#define BASE_ROOM_NUMBER 5

typedef struct room_node {
  SDL_Rect          room;
  struct room_node  *next;
} room_t;

typedef struct {
  SDL_Renderer  *renderer;
  int           grid_w;
  int           grid_h;
  int           scale;
  int           **grid;
  room_t        *rooms;

} state_t;

/*          tools */
int         random_int(int lower, int upper);

/*          level */
void        init_level(state_t *state, int complexity);
void        free_level(state_t *state);

/*          room */
SDL_Rect    place_new_room(state_t *state, int max_rect_side);
SDL_Rect    g_rect(int grid_w, int grid_h, int w, int h);


/*          ll_rooms */
void        free_rooms(state_t *state);
void        rooms_append(state_t *state, SDL_Rect room);
void        through_list(state_t *state);

/*          grid */
void        init_grid(state_t *state, int complexity);
void        free_grid(state_t *state);
void        draw_grid(state_t *state);

#endif