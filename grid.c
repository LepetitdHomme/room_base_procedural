#include "includes/common.h"

void          init_grid(state_t *state, int complexity) {
  if (state->grid != NULL) {
    free_grid(state);
  }
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

void          reset_grid(state_t *state) {
  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      state->grid[i][j] = EMPTY;
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
