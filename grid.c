#include "includes/common.h"

/* 1 <= complexity <= 7 */
void  init_grid(state_t *state, int complexity) {
  state->scale = 13 - complexity;
  if (state->scale <= 4) {
    state->scale = 4;
  }
  state->grid_w = WINDOW_WIDTH / state->scale;
  state->grid_h = WINDOW_HEIGHT / state->scale;
  state->grid = (int **)malloc(sizeof(int **) * state->grid_w);

  for (int i = 0 ; i < state->grid_w ; i++) {
    state->grid[i] = (int *)malloc(sizeof(int *) * state->grid_h);
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
  }
  free(state->grid);
  state->grid = NULL;
}

void draw_grid(state_t *state) {
  SDL_Rect rect;

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      rect.x = i * state->scale;
      rect.y = j * state->scale;
      rect.w = state->scale;
      rect.h = state->scale;
      SDL_SetRenderDrawColor(state->renderer, state->grid[i][j], state->grid[i][j], state->grid[i][j], 255);
      SDL_RenderDrawRect(state->renderer, &rect);
    }
  }
}
