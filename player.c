#include "includes/common.h"

void          free_player(state_t *state) {
  free(state->player);
  state->player = NULL;
}

void          init_player(state_t *state) {
  state->player = (player_t *)malloc(sizeof(player_t));
  if (state->player == NULL) {
    DEBUG_MSG("Failed to malloc player");
    exit(EXIT_FAILURE);
  }

  // SDL_Surface *tmp = SDL_LoadBMP("assets/sorcerer.bmp");
  // if (tmp == NULL) {
  //   printf("Error SDL_LoadBMP: %s\n", SDL_GetError());
  //   return NULL;
  // }
  // SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 25, 225, 115));
  // player.texture = SDL_CreateTextureFromSurface(app->renderer, tmp);
  // SDL_FreeSurface(tmp);
  // if (player.texture == NULL) {
  //   printf("Error SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
  //   return NULL;
  // }
  // SDL_QueryTexture(player.texture, NULL, NULL, &player.original.w, &player.original.h);
  
  state->player->delta_y = 0;
  state->player->delta_x = 0;

  state->player->current_node = state->graph;
  state->player->pos.x = state->graph->center.x;
  state->player->pos.y = state->graph->center.y;
  state->player->direction = RIGHT;
  state->player->last_update = 0;
  
  state->player->src_screen.x = -1;
  state->player->src_screen.y = -1;
  state->player->src_screen.w = -1;
  state->player->src_screen.h = -1;

  state->player->dst_screen.x = -1;
  state->player->dst_screen.y = -1;
  state->player->dst_screen.w = -1;
  state->player->dst_screen.h = -1;
  // state->player->elements = NULL;
  // state->player->weapons = NULL;
}

/***************************************************************/

void          player_update_direction(player_t *player) {
  if (abs(player->delta_x) >= abs(player->delta_y) && player->delta_x != 0) {
    player->direction = (player->delta_x > 0) ? RIGHT : LEFT;
  }
  else if (abs(player->delta_y) > abs(player->delta_x) && player->delta_y != 0) {
    player->direction = (player->delta_y > 0) ? DOWN : UP;
  }
}

void          player_update_node(player_t *player) {
  if (is_in_room(player->pos, player->current_node->rect) == 0) {
    return;
  }

  graph_t *parent = player->current_node->parent;
  if (parent != NULL && is_in_room(player->pos, player->current_node->parent->rect) == 0) {
    player->current_node = parent;
    return;
  }

  for (int i = 0 ; i < player->current_node->num_children ; i++) {
    if (is_in_room(player->pos, player->current_node->children[i]->rect) == 0) {
      player->current_node = player->current_node->children[i];
      return;
    }
  }
}


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

int           player_move_attempt(state_t *state, int dx, int dy) {
  SDL_Rect    test;

  test = state->player->dst_screen;

  test.x += dx;
  test.y += dy;
  // TODO: width and height to update with player texture sizes
  test.w = state->tile_screen_size;
  test.h = state->tile_screen_size;

  if (is_colliding(state, test) == 0) {
    state->player->dst_screen = test;
    state->player->pos.x = state->player->dst_screen.x / state->tile_screen_size;
    state->player->pos.y = state->player->dst_screen.y / state->tile_screen_size;
    player_update_direction(state->player);
    player_update_node(state->player);
    return 1;
  }
  return 0;
}

void          player_move(state_t *state, int dx, int dy) {
  Uint32 delta_player;

  delta_player = state->ticks - state->player->last_update;

  /* TODO: BRUT values */
  if (delta_player >= 20 || state->ticks == -1) {
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
