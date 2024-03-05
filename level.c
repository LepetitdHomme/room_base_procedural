#include "includes/common.h"

int           max_rect_side(int x, int y, int n);

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

void          node_to_grid(state_t *state, graph_t *node, int elevation) {
  for (int i = node->rect.x ; i < node->rect.x + node->rect.w ; i++) {
    for (int j = node->rect.y ; j < node->rect.y + node->rect.h ; j++) {
      if (state->grid[i][j] != 0 || node->elevation != elevation)
        continue;
      if (is_room_wall(node->rect, i, j) == 0) {
        state->grid[i][j] = wall_type(node->rect, i, j);
      } else {
        state->grid[i][j] = FLOOR; // floor
      }
      // if (node->id == 0) { //starting point
      //   state->grid[node->center.x][node->center.y] = EMPTY;
      // }
    }
  }

  for (int i = 0 ; i < node->num_doors ; i++) {
    if (node->elevation != elevation)
      continue;
    state->grid[node->doors[i].coord.x][node->doors[i].coord.y] = door_dir_to_type(node->doors[i].dir);
  }

  for (int i = 0 ; i < node->num_children ; i++) {
    node_to_grid(state, node->children[i], elevation);
  }
}

void          level_to_grid(state_t *state) {
  int         elevation = state->player->current_node->elevation;

  reset_grid(state);
  // we only compute current elevation, this avoids corridors erasing rooms/nodes on the grid
  node_to_grid(state, state->graph, elevation);
}
