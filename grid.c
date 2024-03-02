#include "includes/common.h"

void          init_grid(state_t *state, int complexity) {
  state->scale = SCALE;

  state->grid_w = state->scale * complexity;
  state->grid_h = state->scale * complexity;
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

void          free_grid(state_t *state) {
  for(int i = 0; i < state->grid_w ; i++) {
    free(state->grid[i]);
    state->grid[i] = NULL;
  }
  free(state->grid);
  state->grid = NULL;
} 

void          draw_connections(state_t *state) {
  room_t *tmp = state->rooms;
  door_t *tmp_d = NULL;

  int start_x = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_WIDTH - WINDOW_HEIGHT) / 2 : 0;
  int start_y = (WINDOW_HEIGHT > WINDOW_WIDTH) ? (WINDOW_HEIGHT - WINDOW_WIDTH) / 2 : 0;
  int tile_final_size = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_HEIGHT / state->grid_w) : (WINDOW_WIDTH / state->grid_w);

  while (tmp) {
    tmp_d = tmp->doors;
    while (tmp_d) {
      coord_t a,b;
      a.x = tmp->center.x * tile_final_size + start_x;
      a.y = tmp->center.y * tile_final_size + start_y;
      b.x = tmp_d->room->center.x * tile_final_size + start_x;
      b.y = tmp_d->room->center.y * tile_final_size + start_y;
      SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
      SDL_RenderDrawLine(state->renderer, a.x, a.y, b.x, b.y);
      tmp_d = tmp_d->next;
    }
    tmp = tmp->next;
  }
}

void          draw_level(state_t *state) {
  SDL_Rect rect;
  SDL_Color color;

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

  int start_x = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_WIDTH - WINDOW_HEIGHT) / 2 : 0;
  int start_y = (WINDOW_HEIGHT > WINDOW_WIDTH) ? (WINDOW_HEIGHT - WINDOW_WIDTH) / 2 : 0;
  int tile_final_size = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_HEIGHT / state->grid_w) : (WINDOW_WIDTH / state->grid_w);

  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      rect.x = i * tile_final_size + start_x;
      rect.y = j * tile_final_size + start_y;
      rect.w = tile_final_size;
      rect.h = tile_final_size;

      color = pick_color(state, i, j);
      SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, color.a);
      SDL_RenderFillRect(state->renderer, &rect);
    }
  }
}

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

  // for (int i = room->room.x - 1; i < room->room.x + room->room.w + 1; i++) {
  //   for (int j = room->room.y - 1 ; j < room->room.y + room->room.h + 1; j++) {

  //     if (i < room->room.x || i >= room->room.x + room->room.w || j < room->room.y || j >= room->room.y + room->room.h) {
  //       src = grid_value_to_tileset_rect(state, EMPTY);
  //       tile_type = EMPTY;
  //     } else {
  //       src = grid_value_to_tileset_rect(state, state->grid[i][j]);
  //       tile_type = state->grid[i][j];
  //     }

  //     dst.x = (i - (room->room.x - 1)) * cell_size_float;
  //     dst.y = (j - (room->room.y - 1)) * cell_size_float;
  //     dst.w = cell_size;
  //     dst.h = cell_size;
    //   SDL_RenderCopyEx(state->renderer, state->level_texture->texture, &src, &dst, angle_from_type(tile_type), &center, flip);
    // }
  // }
}
