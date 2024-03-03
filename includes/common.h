#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>// ?
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define DEBUG_MSG(message) printf("Debug: %s, File %s, Line %d\n", message, __FILE__, __LINE__)

#define DEBUG 0

/* WINDOW/SCREEN */
#define WINDOW_WIDTH 1200
#define RATIO_WIDTH 16
#define RATIO_HEIGHT 9
#define WINDOW_HEIGHT (WINDOW_WIDTH * RATIO_HEIGHT) / RATIO_WIDTH

/* GRID GENERATION */
#define SCALE 100
#define MIN_COMPLEXITY 1
#define MAX_COMPLEXITY 10

/* ROOM GENERATION */
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 10
#define DISTANCE_BETWEEN_ROOMS 3
#define BASE_ROOM_NUMBER 10

enum Type { EMPTY, WALL_UP, WALL_DOWN, WALL_LEFT, WALL_RIGHT, CORNER_TOP_LEFT, CORNER_TOP_RIGHT, CORNER_BOT_LEFT, CORNER_BOT_RIGHT, FLOOR, DOOR_UP, DOOR_DOWN, DOOR_LEFT, DOOR_RIGHT, DOOR_SRC, DOOR_DST, CORRIDOR };

enum Dir { UP, DOWN, LEFT, RIGHT };

enum Orientation { VERTICAL, HORIZONTAL };

struct door_node;
struct player_struct;
struct corridor_node;
struct graph_node;

typedef struct {
  int                   x,y;
} coord_t;

typedef struct connection {
  int                   src,dst;
} con_t;

typedef struct {
  int                   src,dst;
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
  coord_t               coord;
  enum Dir              dir;
} door_t;

// typedef struct corridor_node {
//   SDL_Rect              rect;
//   coord_t               center;
//   struct corridor_node  *next;
// } corridor_t;

typedef struct graph_node {
  struct graph_node     *parent;
  struct graph_node     **children;
  int                   num_children;
  SDL_Rect              rect;
  coord_t               center;
  door_t                *doors;
  int                   num_doors;
  int                   is_corridor;
  int                   id;
} graph_t;

typedef struct {
  SDL_Texture           *texture;
  int                   tile_w,tile_h;
  int                   num_tiles_x,num_tiles_y;
} texture_t;

typedef struct {
  unsigned int          seed;
  SDL_Renderer          *renderer;
  texture_t             *level_texture;
  int                   **grid;
  graph_t               *graph;
  room_t                *rooms;
  struct player_struct  *player;
  int                   grid_w,grid_h;
  int                   scale;
  int                   num_rooms;
  con_t                 *connections;
  int                   num_connections;
} state_t;

typedef struct player_struct {
  coord_t               pos;
  room_t                *room;
  enum Dir              direction;
} player_t;


/*                      grid */
void                    init_grid(state_t *state, int complexity);
void                    free_grid(state_t *state);
void                    reset_grid(state_t *state);
void                    draw_level(state_t *state);
// void                    draw_connections(state_t *state);
void                    draw_grid(state_t *state);

/*                      level */
void                    init_level(state_t *state, int complexity);
// void                    level_into_grid(state_t *state);
void                    free_level(state_t *state);
void                    node_to_grid(state_t *state, graph_t *node);
void                    level_to_grid(state_t *state);

/*                      room */
int                     is_room_wall(SDL_Rect room, int i, int j);
room_t                  *find_room_by_id(state_t *state, int id);
enum Type               wall_type(SDL_Rect room, int x, int y);
SDL_Rect                place_new_room(state_t *state, int max_rect_side);
coord_t                 room_center(SDL_Rect room);
SDL_Rect                g_rect(int grid_w, int grid_h, int w, int h);
void                    free_rooms(state_t *state);
void                    rooms_append(state_t *state, SDL_Rect room, int id);

/*                      connections */
void                    connections_append(state_t *state, int src, int dst);
void                    free_connections(state_t *state);
void                    connections_print(state_t *state);

/*                      door */
void                    doors_append(graph_t *src_node, door_t door_node);
void                    free_doors(graph_t *node);
enum Dir                door_dir(SDL_Rect room, int x, int y);
door_t                  door_coordinates(graph_t *src, graph_t *dst);
SDL_Rect                rect_from_doors(door_t src, door_t dst);

/*                      corridors */
// void                    free_corridors(door_t *door);
// void                    door_to_door(door_t *door, room_t *room1, room_t *room2);
// void                    corridors_append(door_t *door, room_t *room1, room_t *room2);

/*                      graph */
void                    graph_create(state_t *state);
graph_t                 *graph_create_node(SDL_Rect rect, coord_t center, int is_corridor, int id);
graph_t                 *graph_create_node_from_connection(state_t *state, graph_t *parent_node, graph_t *child_node);
graph_t                 *graph_create_node_from_room(state_t *state, con_t *connections, int num_connections, room_t *room);
void                    graph_add_child(graph_t *parent, graph_t *child);
void                    free_graph(state_t *state);
void                    displayGraph(graph_t *node, int depth);

/*                      player */
void                    init_player(state_t *state);

/*                      textures */
void                    init_texture(state_t *state, const char *path, int num_x, int num_y);
void                    free_texture(state_t *state);
SDL_Rect                grid_value_to_tileset_rect(state_t *state, int x);

/*                      tools */
int                     clamp(int value, int min, int max);
int                     random_int(int lower, int upper);
double                  distance_between_coords(coord_t center_1, coord_t center_2);

/*                      kruskal */
void                    apply_kruskal(state_t *state);

/*                      type */
SDL_Color               pick_color(state_t *state, int i, int j);
double                  angle_from_type(enum Type type);
enum Type               wall_type(SDL_Rect room, int x, int y);

#endif