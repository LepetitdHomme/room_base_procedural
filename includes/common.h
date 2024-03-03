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
#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
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
#define MIN_CORRIDOR_SIZE 3
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 10
#define DISTANCE_BETWEEN_ROOMS 3
#define BASE_ROOM_NUMBER 10

/* PLAYER */
#define PLAYER_SPEED 7

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
} room_t;

typedef struct door_node {
  coord_t               coord;
  enum Dir              dir;
} door_t;

typedef struct graph_node {
  struct graph_node     *parent;
  struct graph_node     **children;
  int                   num_children;
  SDL_Rect              rect;
  coord_t               center;
  door_t                *doors;
  int                   num_doors;
  int                   is_corridor;
  int                   elevation;
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
  Uint32                ticks;
  texture_t             *level_texture; // contains sdl texture from tileset
  coord_t               zoom; // default value aerial distance from player = number of tiles displayed before and after player
  int                   tile_screen_size; // precomputed tile size from WINDOW_WIDTH and default zoom
  SDL_Point             center_tile; // precomputed tile size center
  SDL_RendererFlip      flip; // basic SDL flip for rendercopyex
  int                   **grid; // the background grid retaining level infos
  int                   grid_w,grid_h;
  room_t                *rooms; // the linked list of generated rooms, NULL after level graph generation
  int                   num_rooms;
  con_t                 *connections; // the number of tuples room-room, necessary for graph
  int                   num_connections;
  graph_t               *graph; // the end result of level structure
  struct player_struct  *player;
  coord_t               scroll; // the top-left position of the scrolling window (not in SDL_Rect because of SDL_Rect limitation)
  int                   scroll_limit_x, scroll_limit_y, scroll_limit_w, scroll_limit_h; // the actual size of the scrolling window
  int                   scale;
  int                   display_map;
} state_t;

typedef struct player_struct {
  coord_t               pos; // grid values
  SDL_Rect              src_screen; // screen values, current pos on screen
  SDL_Rect              dst_screen; // screen values, attempted pos on screen
  int                   delta_x,delta_y; // these are screen values
  int                   speed; // used to increment delta_x, based on state->tile_screen_size
  Uint32                last_update; // last move update ticks
  graph_t               *current_node; // current room/node on graph
  enum Dir              direction;
} player_t;

/*                      draw */
void                    draw_grid(state_t *state);
void                    draw_node(state_t *state);
void                    draw_entities(state_t *state);
void                    compute_screen_sizes(state_t *state);

/*                      grid */
void                    init_grid(state_t *state, int complexity);
void                    free_grid(state_t *state);
void                    reset_grid(state_t *state);

/*                      level */
void                    init_level(state_t *state, int complexity);
void                    node_to_grid(state_t *state, graph_t *node, int elevation);
void                    level_to_grid(state_t *state);

/*                      room */
int                     is_corner_wall(SDL_Rect rect, int i, int j);
int                     is_room_wall(SDL_Rect room, int i, int j);
room_t                  *find_room_by_id(state_t *state, int id);
enum Type               wall_type(SDL_Rect room, int x, int y);
int                     room_is_valid(state_t *state, SDL_Rect room, int spacing, int min_size);
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
coord_t                 next_coord_with_step(coord_t src, enum Dir dir);
door_t                  door_coordinates(graph_t *src, graph_t *dst);
SDL_Rect                rect_from_doors(door_t src, door_t dst);

/*                      graph */
void                    graph_create(state_t *state);
graph_t                 *graph_create_node(SDL_Rect rect, coord_t center, int is_corridor, int elevation, int id);
graph_t                 *graph_create_node_from_connection(state_t *state, graph_t *parent_node, graph_t *child_node);
graph_t                 *graph_create_node_from_room(state_t *state, con_t *connections, int num_connections, room_t *room);
void                    graph_add_child(graph_t *parent, graph_t *child);
void                    free_node(graph_t *node);
void                    free_graph(state_t *state);
void                    graph_print(graph_t *node, int depth);

/*                      player */
void                    init_player(state_t *state);
void                    free_player(state_t *state);
void                    player_update_direction(player_t *player);
void                    player_refine_move_attempt(state_t *state, int dx, int dy);
void                    player_move(state_t *state, int dx, int dy);
int                     player_move_attempt(state_t *state, int dx, int dy);

/*                      entities */

/*                      collisions */
int                     is_colliding(state_t *state, SDL_Rect dst);

/*                      textures */
void                    init_texture(state_t *state, const char *path, int num_x, int num_y);
void                    free_texture(state_t *state);
SDL_Rect                grid_value_to_tileset_rect(state_t *state, int x);

/*                      inputs */
void                    inputs_handle(state_t *state, int *quit, SDL_Event *event, int *inputs_state);
void                    inputs_keydown(state_t *state, SDL_Event *event, int *inputs_state);

/*                      tools */
int                     clamp(int value, int min, int max);
int                     random_int(int lower, int upper);
double                  distance_between_coords(coord_t center_1, coord_t center_2);

/*                      kruskal */
void                    apply_kruskal(state_t *state);

/*                      type */
enum Dir                invert_dir(enum Dir dir);
enum Type               door_dir_to_type(enum Dir dir);
SDL_Color               pick_color(state_t *state, int i, int j);
double                  angle_from_type(enum Type type);
enum Type               wall_type(SDL_Rect room, int x, int y);

#endif