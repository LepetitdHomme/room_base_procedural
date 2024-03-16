#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>
#include "enums.h"

struct door_node;
struct player_struct;
struct corridor_node;
struct graph_node;

typedef struct {
  int                   x,y;
} coord_t;

typedef struct {
  coord_t               a,b;
} line_t;

typedef struct {
  SDL_Vertex            a;
  SDL_Vertex            b;
  float                 angle_a;
  float                 angle_b;
  int                   hit;
} light_edge;

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
  struct graph_node     *dst_node;
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
  int                   visited;
  int                   **light_map;
  int                   id;
} graph_t;

typedef struct {
  SDL_Rect              src;
} tile_prop_t;

typedef struct {
  SDL_Texture           *texture;
  int                   tile_w,tile_h;
  int                   num_tiles_x,num_tiles_y;
  tile_prop_t           *props;
} texture_t;

typedef struct {
  coord_t               a,b,c;
} triangle_t;

typedef struct {
  unsigned int          seed;
  SDL_Renderer          *renderer;
  Uint32                ticks;
  texture_t             *level_texture; // contains sdl texture from tileset
  SDL_Texture           *black_texture;
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
  texture_t             *texture;
  SDL_Rect              src_screen; // screen values, current pos on screen
  SDL_Rect              dst_screen; // screen values, attempted pos on screen
  coord_t               pos; // grid values
  int                   delta_x,delta_y; // these are screen values
  int                   speed; // used to increment delta_x, based on state->tile_screen_size
  Uint32                last_update; // last move update ticks
  graph_t               *current_node; // current room/node on graph
  enum Dir              direction;
  int                   light;
} player_t;

#endif
