#include "includes/common.h"

void          draw_grid(state_t *state) {
  SDL_Rect          src,dst;
  SDL_Color         color;
  SDL_Point         center;
  SDL_RendererFlip  flip;
  enum Type         tile_type;
  int               start_x, start_y, tile_final_size;

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
  flip = SDL_FLIP_NONE;
  start_x = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_WIDTH - WINDOW_HEIGHT) / 2 : 0;
  start_y = (WINDOW_HEIGHT > WINDOW_WIDTH) ? (WINDOW_HEIGHT - WINDOW_WIDTH) / 2 : 0;
  tile_final_size = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_HEIGHT / state->grid_w) : (WINDOW_WIDTH / state->grid_w);
  center.x = tile_final_size / 2;
  center.y = tile_final_size / 2;

  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      dst.x = i * tile_final_size + start_x;
      dst.y = j * tile_final_size + start_y;
      dst.w = tile_final_size;
      dst.h = tile_final_size;
      tile_type = state->grid[i][j];
      src = grid_value_to_tileset_rect(state, tile_type);
      // SDL_RenderCopy(state->renderer, state->level_texture->texture, &src, &dst);
      SDL_RenderCopyEx(state->renderer, state->level_texture->texture, &src, &dst, angle_from_type(tile_type), &center, flip);
    }
  }
}

int           clamp_scroll(state_t *state) {
  if (state->scroll.x < 0)
    state->scroll.x = 0;
  if (state->scroll.y < 0)
    state->scroll.y = 0;
  if (state->scroll.x > state->grid_w * state->tile_screen_size - WINDOW_WIDTH - 1)
    state->scroll.x = state->grid_w * state->tile_screen_size - WINDOW_WIDTH - 1;
  if (state->scroll.y > state->grid_h * state->tile_screen_size - WINDOW_HEIGHT -1)
    state->scroll.y = state->grid_h * state->tile_screen_size - WINDOW_HEIGHT -1;
}

int           update_scroll(state_t *state) {
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
  clamp_scroll(state);
}

/* must be done after level/player init */
void          compute_screen_sizes(state_t *state) {
  // take specific distance rect before and after player = ZOOM
  state->zoom.x = 10;
  // compute the final tile size based on WINDOW WIDTH and ZOOM
  state->tile_screen_size = WINDOW_WIDTH / (state->zoom.x * 2);
  // compute the number of y tiles we want to display based on tile screen size
  state->zoom.y = WINDOW_HEIGHT / (state->tile_screen_size * 2);
  // get center of tiles on screen
  state->center_tile.x = state->tile_screen_size / 2;
  state->center_tile.y = state->tile_screen_size / 2;
  state->flip = SDL_FLIP_NONE;
  // compute player screen position
  state->player->dst_screen.x = state->player->pos.x * state->tile_screen_size;
  state->player->dst_screen.y = state->player->pos.y * state->tile_screen_size;
  // player speed - related to movement and tile size, since player is displayed screen-wise, not grid-wise
  state->player->speed = state->tile_screen_size / 4;
  // compute scrolling window
  // TODO: BRUT values
  state->scroll.x = state->player->dst_screen.x - (state->zoom.x * state->tile_screen_size);
  state->scroll.y = state->player->dst_screen.y - (state->zoom.y * state->tile_screen_size);
  state->scroll_limit_x = 300;
  state->scroll_limit_y = 300;
  state->scroll_limit_w = WINDOW_WIDTH - 600;
  state->scroll_limit_h = WINDOW_HEIGHT - 400;
}

void          draw_entities(state_t *state) {
  SDL_Rect    player;
  coord_t     start_grid;

  player.x = state->player->dst_screen.x - state->scroll.x;
  player.y = state->player->dst_screen.y - state->scroll.y;
  player.w = state->tile_screen_size / 2;
  player.h = state->tile_screen_size;

  SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
  SDL_RenderDrawRect(state->renderer, &player);
}

void          draw_node(state_t *state) {
  graph_t           *node = state->player->current_node;
  SDL_Rect          dst_screen;
  SDL_Rect          src_texture;
  enum Type         tile_type;
  int               minx, maxx, miny, maxy;

  update_scroll(state);

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
      src_texture = grid_value_to_tileset_rect(state, tile_type);

      dst_screen.x = i * state->tile_screen_size - state->scroll.x;
      dst_screen.y = j * state->tile_screen_size - state->scroll.y;
      dst_screen.w = state->tile_screen_size;
      dst_screen.h = state->tile_screen_size;
      SDL_RenderCopyEx(state->renderer, state->level_texture->texture, &src_texture, &dst_screen, angle_from_type(tile_type), &state->center_tile, state->flip);
    }
  }
}
