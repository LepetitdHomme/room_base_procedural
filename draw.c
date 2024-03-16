#include "includes/common.h"

/*
*   For now, used to display the whole grid
*     (the one computed by level_to_grid)
*/
void          draw_grid(state_t *state) {
  SDL_Rect          dst,player;
  SDL_Color         color;
  int               start_x, start_y, tile_final_size;

  start_x = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_WIDTH - WINDOW_HEIGHT) / 2 : 0;
  start_y = (WINDOW_HEIGHT > WINDOW_WIDTH) ? (WINDOW_HEIGHT - WINDOW_WIDTH) / 2 : 0;
  tile_final_size = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_HEIGHT / state->grid_w) : (WINDOW_WIDTH / state->grid_w);

  SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);

  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      if (state->grid[i][j] == EMPTY || is_door_type(state->grid[i][j]) == 0) {
        continue;
      }
      dst.x = i * tile_final_size + start_x;
      dst.y = j * tile_final_size + start_y;
      dst.w = tile_final_size;
      dst.h = tile_final_size;
      color = type_to_map_color(state->grid[i][j]);
      SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, 70);
      SDL_RenderFillRect(state->renderer, &dst);
    }
  }
  SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 128);
  player.x = state->player->pos.x * tile_final_size + start_x;
  player.y = state->player->pos.y * tile_final_size + start_y;
  player.w = tile_final_size;
  player.h = tile_final_size;
  SDL_RenderFillRect(state->renderer, &player);
  SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_NONE);
}

void          draw_map_node(state_t *state, graph_t *node, int tile_final_size, int start_x, int start_y) {
  coord_t     current_grid_coord;
  SDL_Rect    dst;
  SDL_Color   color;

  if (node->visited == 1) {
    for (int i = node->rect.x ; i < node->rect.x + node->rect.w ; i++) {
      for (int j = node->rect.y ; j < node->rect.y + node->rect.h ; j++) {
        if (state->grid[i][j] == EMPTY || is_door_type(state->grid[i][j]) == 0) {
          continue;
        }

        current_grid_coord.x = i;
        current_grid_coord.y = j;
        if (squared_distance_between_coords(current_grid_coord, state->player->pos) > 250) {
          continue;
        }

        dst.x = i * tile_final_size + start_x;
        dst.y = j * tile_final_size + start_y;
        dst.w = tile_final_size;
        dst.h = tile_final_size;
        color = type_to_map_color(state->grid[i][j]);
        SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, 70);
        SDL_RenderFillRect(state->renderer, &dst);
      }
    }
  }

  for (int k = 0 ; k < node->num_children ; k++) {
    draw_map_node(state, node->children[k], tile_final_size, start_x, start_y);
  }
}

/*
*   For now, used to display the whole grid
*     (the one computed by level_to_grid)
*/
void          draw_map_grid(state_t *state) {
  SDL_Rect          player;
  int               start_x, start_y, tile_final_size;

  start_x = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_WIDTH - WINDOW_HEIGHT) / 2 : 0;
  start_y = (WINDOW_HEIGHT > WINDOW_WIDTH) ? (WINDOW_HEIGHT - WINDOW_WIDTH) / 2 : 0;
  tile_final_size = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_HEIGHT / state->grid_w) : (WINDOW_WIDTH / state->grid_w);

  // start_x = 0;
  // start_y = 0;
  // tile_final_size = 5;

  // printf("x: %d, y: %d, size: %d\n", start_x, start_y, tile_final_size);

  SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);

  // display player on map
  SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 128);
  player.x = state->player->pos.x * tile_final_size + start_x;
  player.y = state->player->pos.y * tile_final_size + start_y;
  player.w = tile_final_size;
  player.h = tile_final_size;
  SDL_RenderFillRect(state->renderer, &player);

  draw_map_node(state, state->graph, tile_final_size, start_x, start_y);

  SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_NONE);
}

void          draw_clamp_scroll(state_t *state) {
  if (state->scroll.x < 0)
    state->scroll.x = 0;
  if (state->scroll.y < 0)
    state->scroll.y = 0;
  if (state->scroll.x > state->grid_w * state->tile_screen_size - WINDOW_WIDTH - 1)
    state->scroll.x = state->grid_w * state->tile_screen_size - WINDOW_WIDTH - 1;
  if (state->scroll.y > state->grid_h * state->tile_screen_size - WINDOW_HEIGHT -1)
    state->scroll.y = state->grid_h * state->tile_screen_size - WINDOW_HEIGHT -1;
}

void          draw_update_scroll(state_t *state) {
  int         center_p_x, center_p_y, limit_x_min, limit_y_min, limit_x_max, limit_y_max;

  center_p_x = state->player->dst_screen.x + state->player->dst_screen.w / 2;
  center_p_y = state->player->dst_screen.y + state->player->dst_screen.h / 2;

  limit_x_min = state->scroll.x + state->scroll_limit_x;
  limit_x_max = limit_x_min + state->scroll_limit_w;

  limit_y_min = state->scroll.y + state->scroll_limit_y;
  limit_y_max = limit_y_min + state->scroll_limit_h;

  if (center_p_x < limit_x_min)
    state->scroll.x -= (limit_x_min - center_p_x);
  if (center_p_y < limit_y_min)
    state->scroll.y -= (limit_y_min - center_p_y);
  if (center_p_x > limit_x_max)
    state->scroll.x += (center_p_x - limit_x_max);
  if (center_p_y > limit_y_max)
    state->scroll.y += (center_p_y - limit_y_max);
  // draw_clamp_scroll(state);
}

/*
*   must be done after level/player init
*/
void          draw_compute_screen_sizes(state_t *state) {
  // take specific distance rect before and after player = ZOOM
  if (state->zoom.x == -1) {
    state->zoom.x = 15; // clamped in inputs
  }
  // compute the final tile size based on WINDOW WIDTH and ZOOM
  state->tile_screen_size = WINDOW_WIDTH / (state->zoom.x * 2);
  // compute the number of y tiles we want to display based on tile screen size
  state->zoom.y = WINDOW_HEIGHT / (state->tile_screen_size * 2);
  // get center of tiles on screen
  state->center_tile.x = state->tile_screen_size / 2;
  state->center_tile.y = state->tile_screen_size / 2;
  state->flip = SDL_FLIP_NONE;
  player_reset_screen_from_grid(state);  
}

void          draw_scrolling_window(state_t *state) {
  SDL_Rect scroll_display;
  scroll_display.x = state->scroll_limit_x;
  scroll_display.y = state->scroll_limit_y;
  scroll_display.w = state->scroll_limit_w;
  scroll_display.h = state->scroll_limit_h;
  SDL_SetRenderDrawColor(state->renderer, 0, 255, 0, 255);
  SDL_RenderDrawRect(state->renderer, &scroll_display);  
}

void          draw_entities(state_t *state) {
  SDL_Rect    player;

  // player is its own collision box ; a quarter tile ; top left
  // we draw his sprite around his collision box , which is its position
  // its sprite is basically a tile, so:
  player.x = state->player->dst_screen.x - state->scroll.x;
  player.y = state->player->dst_screen.y - state->scroll.y;
  player.w = state->player->dst_screen.w;
  player.h = state->player->dst_screen.h;
  // around collision box:
  player.x -= state->tile_screen_size / 4;
  player.y -= player.h;
  player.w *= 2;
  player.h *= 2;

  SDL_RenderCopy(state->renderer, state->player->texture->texture, &state->player->src_screen, &player);

  if (DEBUG_COLLISIONS) {
    SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
    player.x = (state->player->dst_screen.x) - state->scroll.x; // center of width
    player.y = (state->player->dst_screen.y) - state->scroll.y; // feet
    player.w = state->player->dst_screen.w;
    player.h = state->player->dst_screen.h;
    SDL_RenderDrawRect(state->renderer, &player);
  }
}

/*
*   remember that there are 3 levels of coords:
*   grid: grid
*   world: grid * tile
*   window: grid * tile size - scroll
*/
void          draw_node(state_t *state) {
  graph_t           *node = state->player->current_node;
  SDL_Rect          dst_screen;
  tile_prop_t       src_tile;
  enum Type         tile_type;
  int               minx, maxx, miny, maxy;

  draw_update_scroll(state);

  minx = state->scroll.x / state->tile_screen_size - 1;
  miny = state->scroll.y / state->tile_screen_size - 1;
  maxx = (state->scroll.x + WINDOW_WIDTH) / state->tile_screen_size;
  maxy = (state->scroll.y + WINDOW_HEIGHT) / state->tile_screen_size;

  for (int i = minx ; i <= maxx ; i++) {
    for (int j = miny; j <= maxy ; j++) {
      if (i < 0 || i >= state->grid_w || j < 0 || j >= state->grid_h) {
        continue;
      }

      if (i < node->rect.x || i >= node->rect.x + node->rect.w || j < node->rect.y || j >= node->rect.y + node->rect.h) {
        tile_type = EMPTY;
      } else {
        tile_type = state->grid[i][j];
      }
      // src_texture = grid_value_to_tileset_rect(state, tile_type);
      // tile_flip = flip_from_type(tile_type);
      src_tile = state->level_texture->props[tile_type];
      dst_screen.x = i * state->tile_screen_size - state->scroll.x;
      dst_screen.y = j * state->tile_screen_size - state->scroll.y;
      dst_screen.w = state->tile_screen_size;
      dst_screen.h = state->tile_screen_size;
      SDL_RenderCopyEx(state->renderer, state->level_texture->texture, &src_tile.src, &dst_screen, 0, &state->center_tile, state->flip);
    }
  }
  if (DEBUG_SCREEN) draw_scrolling_window(state);
}
