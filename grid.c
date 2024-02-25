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
    case 0://empty
      color.r = 0;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    case 1://wall
      color.r = 127;
      color.g = 127;
      color.b = 127;
      color.a = 127;
      break;
    case 2://floor
      color.r = 255;
      color.g = 255;
      color.b = 255;
      color.a = 255;
      break;
    case 3://door
      color.r = 255;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    default:
      color.r = 0;
      color.g = 255;
      color.b = 0;
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
      SDL_RenderDrawRect(state->renderer, &rect);
    }
  }
}
