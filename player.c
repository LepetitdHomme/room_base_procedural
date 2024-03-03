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
  
  // state->player->delta_y = 0;
  // state->player->delta_x = 0;
  state->player->current_node = state->graph;
  state->player->pos.x = state->graph->center.x;
  state->player->pos.y = state->graph->center.y;
  state->player->direction = RIGHT;
  state->player->last_update = 0;

  
  // state->player->src.x = 0;
  // state->player->src.y = 0;
  // state->player->src.w = 50;
  // state->player->src.h = 40;
  // TODO: dynamically change with level
  // state->player->dst.x = 70;
  // state->player->dst.y = 768;
  // state->player->dst.w = 16;
  // state->player->dst.h = 48;
  // state->player->elements = NULL;
  // state->player->weapons = NULL;
}

/***************************************************************/

void player_update_direction(player_struct *player)
{
  if (ABS(player->delta_x) >= ABS(player->delta_y) && player->delta_x != 0) {
    player->direction = (player->delta_x > 0) ? RIGHT : LEFT;
  }
  else if (ABS(player->delta_y) > ABS(player->delta_x) && player->delta_y != 0) {
    player->direction = (player->delta_y > 0) ? DOWN : UP;
  }
}

int player_move_attempt(player_struct *player, Map *map, int dx, int dy)
{
  SDL_Rect test;
  struct elements *tmp = NULL;
  test = player->dst;

  test.x += dx;
  test.y += dy;
  if (collisions_is_colliding(player, &test, map) == 0)
  {
    player->dst = test;
    player_update_direction(player);
    /** update weapons  **/
    weapons_stick_to_player(player, SWORD);
    /******************/
    return 1;
  }
  return 0;
}

void player_refine_move_attempt(player_struct *player, Map *map, int dx, int dy)
{
  int i;

  for ( i = 0 ; i < ABS(dx) ; i++)
  {
    if (player_move_attempt(player, map, SGN(dx), 0) == 0) {
      // player->delta_x = 0;
      break;
    }
  }
  for ( i = 0 ; i < ABS(dy) ; i++)
  {
    if (player_move_attempt(player, map, 0, SGN(dy)) == 0) {
      // player->delta_y = 0;
      break;
    }
  }

}

void player_move(state_t *state, int dx, int dy) {
  Uint32 delta_player;

  delta_player = state->ticks - state->player->last_update;

  if (delta_player >= 20 || state->ticks == -1) {
    if (ABS(dx) >= map->TILE_WIDTH || ABS(dy) >= map->TILE_HEIGHT) {
      player_move(player, map, ticks, dx/2, dy/2);
      player_move(player, map, ticks, dx - ABS(dx/2), dy - ABS(dy/2));
      return;
    }
    if (player_move_attempt(player, map, dx, dy) == 0)
      player_refine_move_attempt(player, map, dx, dy);
    last_update_player = ticks;
  }
}

// void player_animate(player_struct *player, Map *map, SDL_Rect dst, Uint32 ticks)
// {
//   static int p_x = 0;
//   static int p_y = 0;
//   static int s_x = 0;
//   static int s_y = 0;
//   static int going_left = 0;
//   int anim = (ticks / 100) % 6;

//   if (dst.x != p_x || dst.y != p_y || s_x != map->scrollx || s_y != map->scrolly) {
//     p_x = dst.x;
//     p_y = dst.y;
//     s_x = map->scrollx;
//     s_y = map->scrolly;
//     player->src.x = anim * player->original.w;
//   }
// }
