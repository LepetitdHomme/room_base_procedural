#include "includes/common.h"

void          free_player(state_t *state) {
  if (state->player->texture != NULL) {
    free_texture(state->player->texture);
    state->player->texture = NULL;
  }
  free(state->player);
  state->player = NULL;
}

void          init_player(state_t *state) {
  if ((state->player = (player_t *)malloc(sizeof(player_t))) == NULL) {
    DEBUG_MSG("Failed to malloc player");
    exit(EXIT_FAILURE);
  }

  // init texture_t player
  if ((state->player->texture = (texture_t *)malloc(sizeof(texture_t))) == NULL) {
    DEBUG_MSG("player texture malloc failed");
    exit(EXIT_FAILURE);
  }
  init_texture(state->renderer, state->player->texture, "assets/soldier2.bmp", 2, 2);
  state->player->src_screen.x = 0;
  state->player->src_screen.y = 0;
  state->player->src_screen.w = state->player->texture->tile_w;
  state->player->src_screen.h = state->player->texture->tile_h;

  state->player->delta_y = 0;
  state->player->delta_x = 0;
  state->player->current_node = state->graph;
  state->player->current_node->visited = 1;
  state->player->pos.x = state->graph->center.x;
  state->player->pos.y = state->graph->center.y;
  state->player->direction = RIGHT;
  state->player->last_update = 0;

  state->player->dst_screen.x = -1;
  state->player->dst_screen.y = -1;
  state->player->dst_screen.w = -1;
  state->player->dst_screen.h = -1;
  // state->player->elements = NULL;
  // state->player->weapons = NULL;
}

void          player_reset_screen_from_grid(state_t *state) {
  player_t    *player;

  player = state->player;    
  // compute player screen position
  player->dst_screen.x = player->pos.x * state->tile_screen_size;
  player->dst_screen.y = player->pos.y * state->tile_screen_size;
  player_update_direction(state->player);
  player->dst_screen.w = state->tile_screen_size;
  player->dst_screen.h = state->tile_screen_size;
  
  // player speed - related to movement and tile size, since player is displayed screen-wise, not grid-wise
  player->speed = state->tile_screen_size / 6;
  /* TODO: BRUT value */
  // player->speed = 6;
  
  // compute scrolling window
  // TODO: BRUT values
  state->scroll.x = player->dst_screen.x - (state->zoom.x * state->tile_screen_size);
  state->scroll.y = player->dst_screen.y - (state->zoom.y * state->tile_screen_size);
  state->scroll_limit_x = WINDOW_WIDTH / 4;
  state->scroll_limit_y = WINDOW_HEIGHT / 4;
  state->scroll_limit_w = WINDOW_WIDTH - (2 * state->scroll_limit_x);
  state->scroll_limit_h = WINDOW_HEIGHT - (2 * state->scroll_limit_y);

}

void          player_update_src_from_direction(player_t *player) {
  player->src_screen.w = player->texture->tile_w;
  player->src_screen.h = player->texture->tile_h;  

  switch (player->direction) {
    case DOWN:
      player->src_screen.x = 0;
      player->src_screen.y = 0;
      break;
    case UP:
      player->src_screen.x = 1 * player->texture->tile_w;
      player->src_screen.y = 0 * player->texture->tile_h;
      break;
    case LEFT:
      player->src_screen.x = 0 * player->texture->tile_w;
      player->src_screen.y = 1 * player->texture->tile_h;
      break;
    case RIGHT:
      player->src_screen.x = 1 * player->texture->tile_w;
      player->src_screen.y = 1 * player->texture->tile_h;
      break;
    default:
      player->src_screen.x = 1 * player->texture->tile_w;
      player->src_screen.y = 1 * player->texture->tile_h;
  }
}

void          player_update_direction(player_t *player) {
  if (abs(player->delta_x) >= abs(player->delta_y) && player->delta_x != 0) {
    player->direction = (player->delta_x > 0) ? RIGHT : LEFT;
  }
  else if (abs(player->delta_y) > abs(player->delta_x) && player->delta_y != 0) {
    player->direction = (player->delta_y > 0) ? DOWN : UP;
  }
  player_update_src_from_direction(player);
}

/*
*   if not in current node rect;
*     checks parent and children then update node when
*     current player rect if found
*/
void          player_update_node(player_t *player) {
  if (is_in_room(player->pos, player->current_node->rect) == 0) {
    return;
  }

  graph_t *parent = player->current_node->parent;
  if (parent != NULL && is_in_room(player->pos, player->current_node->parent->rect) == 0) {
    player->current_node = parent;
    // visited
    return;
  }

  for (int i = 0 ; i < player->current_node->num_children ; i++) {
    if (is_in_room(player->pos, player->current_node->children[i]->rect) == 0) {
      player->current_node = player->current_node->children[i];
      // visited
      return;
    }
  }
}

/*            only for delta/attemps moves */
void          player_move_proceed(state_t *state, SDL_Rect test) {
  // test.x -= state->player->dst_screen.w / 2; // removing collision box
  // test.y -= state->player->dst_screen.h / 2; // removing collision box
  state->player->pos.x = test.x / state->tile_screen_size;
  state->player->pos.y = test.y / state->tile_screen_size;
  state->player->dst_screen = test;
  player_update_direction(state->player);
}

/*
*   The move attempt was detected as a move towards a door
*   current algo wants that: doors are siblings on the grid (src and dst)
*     so we check if the current tile is already a door, then it means
*     the move represents a node/room change
*/
void          player_move_to_door(state_t *state, SDL_Rect test) {
  enum Type     current_tile;
  graph_t       *current_node;
  player_t      *player;

  current_tile = state->grid[state->player->pos.x][state->player->pos.y];
  player = state->player;
  current_node = player->current_node;

  if (is_door_type(current_tile) == 1) { // false
    player_move_proceed(state, test);
    return;
  }

  for (int i = 0 ; i < current_node->num_doors ; i++) {
    if (player->pos.x == current_node->doors[i].coord.x && player->pos.y == current_node->doors[i].coord.y) {
      player->pos = next_coord_with_step(current_node->doors[i].coord, current_node->doors[i].dir);
      player->pos = next_coord_with_step(player->pos, current_node->doors[i].dir);
      // update node
      if (player->current_node->elevation != current_node->doors[i].dst_node->elevation) {
        player->current_node = current_node->doors[i].dst_node;
        player->current_node->visited = 1;
        level_to_grid(state, state->graph);
      } else {
        player->current_node = current_node->doors[i].dst_node;
        player->current_node->visited = 1;
      }
      player_reset_screen_from_grid(state);
      break;
    }
  }
}

/*
*   If player_move_attempt failed
*   we try reducing dx/dy and retry
*/
void          player_refine_move_attempt(state_t *state, int dx, int dy) {
  for (int i = 0 ; i < abs(dx) ; i++) {
    if (player_move_attempt(state, SGN(dx), 0) == 0) {
      // player->delta_x = 0;
      break;
    }
  }
  for (int i = 0 ; i < abs(dy) ; i++) {
    if (player_move_attempt(state, 0, SGN(dy)) == 0) {
      // player->delta_y = 0;
      break;
    }
  }

}

/*
*   We check collisions with the collision box
*   but we update/proceed with current value (not collision value)
*/
int           player_move_attempt(state_t *state, int dx, int dy) {
  SDL_Rect    test;
  SDL_Rect    current_collision_box_pos;
  int         collision;

  current_collision_box_pos.x = state->player->dst_screen.x;
  current_collision_box_pos.y = state->player->dst_screen.y;
  /* Collision box move attempt */
  test.x = current_collision_box_pos.x + dx;
  test.y = current_collision_box_pos.y + dy;
  test.w = state->player->dst_screen.w;
  test.h = state->player->dst_screen.h;

  collision = is_colliding_with(state, test);

  if (collision == FLOOR) {
    player_move_proceed(state, test);
    return 1;
  } else if (is_door_type(collision) == 0) {
    player_move_to_door(state, test);
    return 1;
  }
  return 0;
}

/*
*   attemps to move player
*   based on delta x and delta y (dx, dy)
*   also responsible for collision check down the line
*/
void          player_move(state_t *state, int dx, int dy) {
  Uint32 delta_player;

  delta_player = state->ticks - state->player->last_update;

  /* TODO: BRUT values */
  if (delta_player >= 20) {
    /* used to ensure we move smoothly */
    if (abs(dx) >= state->tile_screen_size || abs(dy) >= state->tile_screen_size) {
      player_move(state, dx/2, dy/2);
      player_move(state, dx - abs(dx/2), dy - abs(dy/2));
      return;
    }
    /* actually move */
    if ((dx != 0 || dy != 0) && player_move_attempt(state, dx, dy) == 0) {
      player_refine_move_attempt(state, dx, dy);
    }
    state->player->last_update = state->ticks;
  }
}
