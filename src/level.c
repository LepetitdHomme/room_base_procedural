#include "../include/level.h"
#include "../include/macros.h"

#include "../include/grid.h"
#include "../include/room.h"
#include "../include/kruskal.h"
#include "../include/graph.h"
#include "../include/player.h"

void          init_level(state_t *state, int complexity) {
  int         rooms_number,max_rect,clamped_complexity;
  SDL_Rect    room;

  clamped_complexity = clamp(complexity, MIN_COMPLEXITY, MAX_COMPLEXITY);
  init_grid(state, clamped_complexity);
  state->num_rooms = BASE_ROOM_NUMBER * clamped_complexity;
  max_rect = max_rect_side(state->grid_w, state->grid_h, state->num_rooms);

  rooms_number = 0;

  while (rooms_number < state->num_rooms) {
    room = place_new_room(state, max_rect);
    if (room.x == -1) {
      continue;
    }
    // add room to state level rooms
    rooms_append(state, room, rooms_number);
    rooms_number++;

  }
  apply_kruskal(state);
  // connections_print(state);
  graph_create(state);
  init_player(state); // => to free
}

int           max_rect_side(int x, int y, int n) {
  double      px = ceil(sqrt(n * x / y));
  double      py = ceil(sqrt(n * y / x));
  double      sx = (floor(px * y / x) * px < n) ? (y / ceil(px * y / x)) : (x / px);
  double      sy = (floor(py * x / y) * py < n) ? (x / ceil(py * x / y)) : (y / py);
  double      max_rect_side = MAX(sx, sy);

  return((int)max_rect_side + 1);
}

void          node_to_grid(state_t *state, graph_t *node, int with_parent, int with_children, int elevation) {
  int         diff_elevation = abs(node->elevation - elevation);

  int test = 0;
  for (int i = node->rect.x ; i < node->rect.x + node->rect.w ; i++) {
    for (int j = node->rect.y ; j < node->rect.y + node->rect.h ; j++) {
      if (state->grid[i][j] != 0 || diff_elevation != 0)
        continue;
      if (is_room_wall(node->rect, i, j) == 0) {
        state->grid[i][j] = wall_type(node->rect, i, j);
      } else {
        if (test == 30 || test == 80 || test == 60) {
          state->grid[i][j] = CORRIDOR; // floor
        } else {
          // state->grid[i][j] = node->is_corridor ? CORRIDOR : FLOOR; // floor
          state->grid[i][j] = FLOOR; // floor
        }
      }
      test++;
    }
  }

  for (int i = 0 ; i < node->num_doors ; i++) {
    if (diff_elevation > 1) continue; // ensures next room doors are rdy for collision !
    state->grid[node->doors[i].coord.x][node->doors[i].coord.y] = door_dir_to_type(node->doors[i].dir);
  }

  if (with_children == 1) {
    for (int i = 0 ; i < node->num_children ; i++) {
      node_to_grid(state, node->children[i], 0, 1, elevation);
    }
  }

  if (with_parent == 1 && node->parent != NULL) {
    node_to_grid(state, node->parent, 0, 0, elevation);
  }
}

/*
*   Starts putting node into grid
*     only diff is that it resets the grid first
*/
void          level_to_grid(state_t *state, graph_t *node) {
  int         elevation = state->player->current_node->elevation;

  reset_grid(state);
  // we only compute current elevation, this avoids corridors erasing rooms/nodes on the grid
  node_to_grid(state, node, 1, 1, elevation);
}
