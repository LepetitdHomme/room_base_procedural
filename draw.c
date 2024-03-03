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
}

void          draw_entities(state_t *state) {
  SDL_Rect    player;
  coord_t     start_grid;

  start_grid.x = state->player->pos.x - state->zoom.x;
  start_grid.y = state->player->pos.y - state->zoom.y;

  player.x = (state->player->pos.x - start_grid.x) * state->tile_screen_size;
  player.y = (state->player->pos.y - start_grid.y) * state->tile_screen_size;
  player.w = state->tile_screen_size;
  player.h = state->tile_screen_size;

  SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);

  SDL_RenderDrawRect(state->renderer, &player);
}

void          draw_node(state_t *state) {
  graph_t           *node = state->player->current_node;
  SDL_Rect          dst_screen;
  SDL_Rect          src_texture;
  coord_t           start_grid;
  enum Type         tile_type;

  // compute starts on the grid
  start_grid.x = state->player->pos.x - state->zoom.x;
  start_grid.y = state->player->pos.y - state->zoom.y;

  // draw everything that is between player - zoom -> player + zoom
  for (int i = start_grid.x ; i < state->player->pos.x + state->zoom.x ; i++) {
    for (int j = start_grid.y ; j < state->player->pos.y + state->zoom.y ; j++) {
      // check if in grid
      if (i < 0 || i >= state->grid_w || j < 0 || j >= state->grid_h) {
        continue;
      }

      // compute tile type + source rect for texture
      // check if outside node
      if (i < node->rect.x || i >= node->rect.x + node->rect.w || j < node->rect.y || j >= node->rect.y + node->rect.h) {
        tile_type = EMPTY;
      } else {
        tile_type = state->grid[i][j];
      }
      src_texture = grid_value_to_tileset_rect(state, tile_type);

      // grid to screen // the constant zoom makes the tiles constant size
      dst_screen.x = (i - start_grid.x) * state->tile_screen_size;
      dst_screen.y = (j - start_grid.y) * state->tile_screen_size;
      dst_screen.w = state->tile_screen_size;
      dst_screen.h = state->tile_screen_size;
      SDL_RenderCopyEx(state->renderer, state->level_texture->texture, &src_texture, &dst_screen, angle_from_type(tile_type), &state->center_tile, state->flip);
    }
  }
}
