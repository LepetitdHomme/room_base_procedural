#include "includes/common.h"

/* 1 <= complexity <= 7 */
void  init_grid(state_t *state, int complexity) {
  state->scale = 13 - complexity;
  if (state->scale <= 4) {
    state->scale = 4;
  }
  state->grid_w = WINDOW_WIDTH / state->scale;
  state->grid_h = WINDOW_HEIGHT / state->scale;
  state->grid = (int **)malloc(sizeof(int *) * state->grid_w);

  for (int i = 0 ; i < state->grid_w ; i++) {
    state->grid[i] = (int *)malloc(sizeof(int) * state->grid_h);
  }

  for(int i = 0; i < state->grid_w ; i++) {
    for(int j = 0; j < state->grid_h ; j++) {
      state->grid[i][j] = 0;
    }
  }
}

void free_grid(state_t *state) {
  for(int i = 0; i < state->grid_w ; i++) {
    free(state->grid[i]);
    state->grid[i] = NULL;
  }
  free(state->grid);
  state->grid = NULL;
}

SDL_Color pick_color(state_t *state, int i, int j) {
  SDL_Color color;

  switch (state->grid[i][j]) {
    case EMPTY://empty
      color.r = 0;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    case WALL://wall
      color.r = 127;
      color.g = 127;
      color.b = 127;
      color.a = 127;
      break;
    case FLOOR://floor
      color.r = 255;
      color.g = 255;
      color.b = 255;
      color.a = 255;
      break;
    case DOOR_SRC://door
      color.r = 0;
      color.g = 255;
      color.b = 0;
      color.a = 255;
      break;
    case DOOR_DST:
      color.r = 255;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    default:
      color.r = 255;
      color.g = 255;
      color.b = 255;
      color.a = 255;
  }

  return color;
}  

void      draw_grid(state_t *state) {
  SDL_Rect rect;
  SDL_Color color;

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      rect.x = i * state->scale;
      rect.y = j * state->scale;
      rect.w = state->scale;
      rect.h = state->scale;

      color = pick_color(state, i, j);
      SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, color.a);
      SDL_RenderFillRect(state->renderer, &rect);
    }
  }
}
void  clamp_scroll(state_t *state) {
  if (state->scroll.x < 0)
    state->scroll.x = 0;
  if (state->scroll.y < 0)
    state->scroll.y = 0;
  if (state->scroll.x > state->grid_w * state->level_texture->tile_w - WINDOW_WIDTH - 1)
    state->scroll.x = state->level_texture->tile_w * state->level_texture->tile_w - WINDOW_WIDTH - 1;
  if (state->scroll.y > state->grid_h * state->level_texture->tile_h - WINDOW_HEIGHT - 1)
    state->scroll.y = state->grid_h * state->level_texture->tile_h - WINDOW_HEIGHT - 1;
}

void  update_scroll(state_t *state) {
  int limit_x_min, limit_y_min, limit_x_max, limit_y_max;

  state->scroll.x = (state->rooms->center.x - 8) * state->level_texture->tile_w;
  state->scroll.y = (state->rooms->center.y - 8) * state->level_texture->tile_h;

  limit_x_min = state->scroll.x + state->limit_scroll.x;
  limit_x_max = limit_x_min + state->limit_scroll.w;

  limit_y_min = state->scroll.y + state->limit_scroll.y;
  limit_y_max = limit_y_min + state->limit_scroll.h;

  if (state->player->pos.x < limit_x_min)
    state->scroll.x -= (limit_x_min - state->player->pos.x);
  if (state->player->pos.y < limit_y_min)
    state->scroll.y -= (limit_y_min - state->player->pos.y);
  if (state->player->pos.x > limit_x_max)
    state->scroll.x += (state->player->pos.x - limit_x_max);
  if (state->player->pos.y > limit_y_max)
    state->scroll.y += (state->player->pos.y - limit_y_max);
  clamp_scroll(state);
}


// BRUT VALUES
SDL_Rect  grid_value_to_tileset_rect(state_t *state, int x) {
  SDL_Rect src;

  switch (x) {
    case EMPTY:
      src.x = 0;
      src.y = 19 * state->level_texture->tile_h;
      break;
    case WALL:
      src.x = 1 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case FLOOR:
      src.x = 10 * state->level_texture->tile_w;
      src.y = 3 * state->level_texture->tile_h;
      break;
    case DOOR_SRC:
      src.x = 22 * state->level_texture->tile_w;
      src.y = 24 * state->level_texture->tile_h;
      break;
    case DOOR_DST:
      src.x = 22 * state->level_texture->tile_w;
      src.y = 25 * state->level_texture->tile_h;
      break;
    default:
      src.x = 0;
      src.y = 0;
  }
  src.w = state->level_texture->tile_w;
  src.h = state->level_texture->tile_h;
  return src;
}  

void      new_draw_grid(state_t *state) {
  SDL_Rect src,dst;
  SDL_Color color;

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

  int minx, maxx, miny, maxy;

  // update_scroll(state);

  minx = state->player->pos.x - SCREEN_TILE;
  maxx = state->player->pos.x + SCREEN_TILE;
  miny = state->player->pos.y - SCREEN_TILE;
  maxy = state->player->pos.y + SCREEN_TILE;

  room_t *room = state->player->room;
  int x,y = 0;
  int grid_cell_to_screen_w = WINDOW_WIDTH / (2 * SCREEN_TILE);
  int grid_cell_to_screen_h = WINDOW_HEIGHT / (2 * SCREEN_TILE);

  printf("trucs: %d - %d - %d - %d\n", minx, miny, maxx, maxy);

  for (int i = minx; i < maxx ; i++) {
    for (int j = miny ; j < maxy ; j++) {
      // if (i < 0 || i >= state->grid_w || j < 0 || j >= state->grid_h) {
      if (i < room->room.x || i >= room->room.x + room->room.w || j < room->room.y || j >= room->room.y + room->room.h) {
        src = grid_value_to_tileset_rect(state, EMPTY);
      } else {
        src = grid_value_to_tileset_rect(state, state->grid[i][j]);
      }

      dst.x = (i - minx) * grid_cell_to_screen_w;
      dst.y = (j - miny) * grid_cell_to_screen_h;
      dst.w = grid_cell_to_screen_w;
      dst.h = grid_cell_to_screen_h;
      SDL_RenderCopy(state->renderer, state->level_texture->texture, &src, &dst);
      y++;
    }
    x++;
  }
}

// int map_display(app_struct *app, Map *map, player_struct *player)
// {
//   int i,j;
//   SDL_Rect dest;
//   int numtile;
//   int minx, maxx, miny, maxy;
//   map_update_scroll(map, player);
//   minx = map->scrollx / (map->TILE_WIDTH);
//   miny = map->scrolly / (map->TILE_HEIGHT);
//   maxx = (map->scrollx + map->screen_w) / (map->TILE_WIDTH);
//   maxy = (map->scrolly + map->screen_h) / (map->TILE_HEIGHT);

//   for ( i = minx ; i <= maxx ; i++)
//   {
//     for ( j = miny ; j <= maxy; j++)
//     {
//       if (i < 0 || i >= map->n_tiles_w || j < 0 || j >= map->n_tiles_h) // the screen goes out of the global map
//         numtile = 0;
//       else
//         numtile = map->schema[i][j];
//       dest.x = i * map->TILE_WIDTH - map->scrollx;
//       dest.y = j * map->TILE_HEIGHT - map->scrolly;
//       dest.w = map->TILE_WIDTH;
//       dest.h = map->TILE_HEIGHT;
//       SDL_RenderCopy(app->renderer, map->tileset, &map->props[numtile].rect, &dest);
//     }
//   }
// }
