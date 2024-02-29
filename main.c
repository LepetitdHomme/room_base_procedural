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
  // through_list(&state);
  int complexity = 1;
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
    if (complexity > 6)
      complexity = 1;
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);
    /* Draw your graphics here (currently an empty black window) */

    init_texture(&state, "assets/dungeon_tileset.bmp", 6, 17);
    init_level(&state, complexity);
    level_into_grid(&state);
    if (DEBUG == 1) {
      draw_level(&state);
      draw_connections(&state);
    } else {
      draw_grid(&state);
    }
    complexity++;
    SDL_RenderPresent(state.renderer);
    sleep(1);
    SDL_Delay(60);
  }

  free_level(&state);

  /* Clean up */
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
