#include "includes/common.h"

int main(int argc, char *argv[]) {
  // Seed for the random number generator
  unsigned int seed;

  if (argc < 2) {
    // If no seed is provided, use the current time as the seed
    seed = (unsigned int)time(NULL);
    printf("No seed provided. Using current time (%u) as seed.\n", seed);
  } else {
    // If a seed is provided, use it
    seed = (unsigned int)atoi(argv[1]);
    printf("Using provided seed: %u\n", seed);
  }
  // Seed the random number generator
  srand(seed);

  printf("Starting SDL2 application\n");
  /* Initialize SDL */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      printf("SDL initialization failed: %s\n", SDL_GetError());
      return 1;
  }

  /* Create a window */
  SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
      printf("Window creation failed: %s\n", SDL_GetError());
      return 1;
  }

  state_t state;
  state.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  state.grid = NULL;
  state.rooms = NULL;
  init_texture(&state, "assets/tileset.bmp", 32, 71);
  init_level(&state, 3); 
  level_into_grid(&state);
  // through_list(&state);

  /* Event loop */
  SDL_Event event;
  int quit = 0;
  while (!quit) {
    /* Handle events */
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = 1;
        }
    }

    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);
    /* Draw your graphics here (currently an empty black window) */

    // draw_grid(&state);
    new_draw_grid(&state);

    SDL_Rect p;
    p.x = state.player->pos.x * state.level_texture->tile_w;
    p.y = state.player->pos.y * state.level_texture->tile_h;
    p.w = state.level_texture->tile_w;
    p.h = state.level_texture->tile_h;
    printf("scroll: %d - %d\n", state.scroll.x, state.scroll.y);
    printf("%d - %d\n", state.player->pos.x, state.player->pos.y);
    printf("%d - %d\n", p.x, p.y);
    // SDL_SetRenderDrawColor(state.renderer, 255, 0, 0, 255);
    // SDL_RenderFillRect(state.renderer, &p);
    // SDL_RenderDrawLine(state.renderer, state.player->pos.x, state.player->pos.y, state.scale, state.scale);
    
    // draw_connections(&state);


    SDL_RenderPresent(state.renderer);

    SDL_Delay(60);
  }

  free_level(&state);

  /* Clean up */
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
