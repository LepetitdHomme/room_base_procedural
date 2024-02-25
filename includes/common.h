#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SIGN(x) ((x > 0) - (x < 0))
#define DEBUG_MSG(message) printf("Debug: %s, File %s, Line %d\n", message, __FILE__, __LINE__)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 10
#define BASE_ROOM_NUMBER 5

enum Type {
  EMPTY,
  WALL,
  FLOOR,
  DOOR_SRC,
  DOOR_DST
};

typedef struct {
  int     x;
  int     y;
} coord_t;

typedef struct {
  int src, dest;
  double weight; // distance
} edge_t;

typedef struct {
  int parent;
  int rank;
} subset_t;

struct door_node;

typedef struct room_node {
  int                   id;
  SDL_Rect              room;
  coord_t               center;
  struct room_node      *next;
  struct door_node      *doors;
} room_t;

typedef struct door_node {
  room_t            *room;
  coord_t           coord_src;
  coord_t           coord_dst;
  struct door_node  *next;
} door_t;

typedef struct {
  SDL_Renderer  *renderer;
  int           grid_w;
  int           grid_h;
  int           scale;
  int           num_rooms;
  int           **grid;
  room_t        *rooms;
} state_t;

/*          tools */
int         random_int(int lower, int upper);
double      distance_between_coords(coord_t center_1, coord_t center_2);

/*          level */
void        draw_connections(state_t *state);
void        init_level(state_t *state, int complexity);
void        level_into_grid(state_t *state);
void        free_level(state_t *state);

/*          room */
int         is_room_wall(SDL_Rect room, int i, int j);
SDL_Rect    place_new_room(state_t *state, int max_rect_side);
coord_t     room_center(SDL_Rect room);
SDL_Rect    g_rect(int grid_w, int grid_h, int w, int h);

/*          ll_rooms */
void        free_doors(room_t *room);
void        free_rooms(state_t *state);
void        rooms_append(state_t *state, SDL_Rect room, int id);
void        doors_append(state_t *state, room_t *src, room_t *dst);
void        through_list(state_t *state);

/*          kruskal */
void        apply_kruskal(state_t *state);

/*          spanning_tree */
void        min_spanning_tree(state_t *state);

/*          grid */
void        init_grid(state_t *state, int complexity);
void        free_grid(state_t *state);
void        draw_grid(state_t *state);

#endif