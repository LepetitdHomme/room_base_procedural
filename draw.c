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
