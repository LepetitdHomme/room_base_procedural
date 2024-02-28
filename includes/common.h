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

#define DEBUG 1

#define WINDOW_WIDTH 1200
#define RATIO_WIDTH 16
#define RATIO_HEIGHT 9
#define WINDOW_HEIGHT (WINDOW_WIDTH * RATIO_HEIGHT) / RATIO_WIDTH

#define SCALE 100
#define MIN_COMPLEXITY 1
#define MAX_COMPLEXITY 10

#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 10
#define DISTANCE_BETWEEN_ROOMS 3
#define BASE_ROOM_NUMBER 10
#define SCREEN_TILE BASE_ROOM_NUMBER * 2

enum Type {
  EMPTY,
  WALL_UP,
  WALL_DOWN,
  WALL_LEFT,
  WALL_RIGHT,
  CORNER_TOP_LEFT,
  CORNER_TOP_RIGHT,
  CORNER_BOT_LEFT,
  CORNER_BOT_RIGHT,
  FLOOR,
  DOOR_UP,
  DOOR_DOWN,
  DOOR_LEFT,
  DOOR_RIGHT,
  DOOR_SRC,
  DOOR_DST,
  CORRIDOR
};

enum Dir {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum Orientation {
  VERTICAL,
  HORIZONTAL
};

struct door_node;
struct player_struct;
struct corridor_node;

typedef struct {
  int                   x,y;
} coord_t;

typedef struct {
  int                   src, dest;
  double                weight; // distance
} edge_t;

typedef struct {
  int                   parent,rank;
} subset_t;

typedef struct room_node {
  int                   id;
  SDL_Rect              room;
  coord_t               center;
  struct room_node      *next;
  struct door_node      *doors;
} room_t;

typedef struct door_node {
  room_t                *room;
  coord_t               coord_src,coord_dst;
  enum Dir              door_src_dir,door_dst_dir;
  struct corridor_node  *corridors;
  struct door_node      *next;
} door_t;

typedef struct corridor_node {
  coord_t               floor,wall_left,wall_right;
  SDL_Rect              rect;
  enum Type             wall_left_type,wall_right_type;
  struct corridor_node  *next;
} corridor_t;

typedef struct {
  SDL_Texture           *texture;
  int                   tile_w,tile_h;
  int                   num_tiles_x,num_tiles_y;
} texture_t;

typedef struct {
  SDL_Renderer          *renderer;
  texture_t             *level_texture;
  int                   grid_w,grid_h;
  int                   scale;
  coord_t               scroll;
  // int                   scroll_w, scroll_h;
  SDL_Rect              limit_scroll;
  int                   num_rooms;
  int                   **grid;
  room_t                *rooms;
  struct player_struct  *player;
} state_t;

typedef struct player_struct {
  coord_t               pos;
  room_t                *room;
  enum Dir              direction;
} player_t;


/*                      textures */
void                    init_texture(state_t *state, const char *path, int num_x, int num_y);
SDL_Rect                grid_value_to_tileset_rect(state_t *state, int x);

/*                      player */
void                    init_player(state_t *state);

/*                      tools */
int                     clamp(int value, int min, int max);
int                     random_int(int lower, int upper);
double                  distance_between_coords(coord_t center_1, coord_t center_2);

/*                      level */
void                    init_level(state_t *state, int complexity);
void                    level_into_grid(state_t *state);
void                    free_level(state_t *state);

/*                      room */
int                     is_room_wall(SDL_Rect room, int i, int j);
enum Dir                door_dir(SDL_Rect room, int x, int y);
enum Type               wall_type(SDL_Rect room, int x, int y);
SDL_Rect                place_new_room(state_t *state, int max_rect_side);
coord_t                 room_center(SDL_Rect room);
SDL_Rect                g_rect(int grid_w, int grid_h, int w, int h);

/*                      corridors */
void                    door_to_door(door_t *door, room_t *room1, room_t *room2);
void                    dig_corridor(door_t *door, room_t *room1, room_t *room2);

/*                      ll_rooms */
void                    free_doors(room_t *room);
void                    free_rooms(state_t *state);
void                    rooms_append(state_t *state, SDL_Rect room, int id);
void                    doors_append(state_t *state, room_t *src, room_t *dst);
void                    through_list(state_t *state);

/*                      kruskal */
void                    apply_kruskal(state_t *state);

/*                      spanning_tree */
void                    min_spanning_tree(state_t *state);

/*                      grid */
void                    init_grid(state_t *state, int complexity);
void                    free_grid(state_t *state);
void                    draw_level(state_t *state);
void                    draw_connections(state_t *state);
void                    draw_grid(state_t *state);

#endif