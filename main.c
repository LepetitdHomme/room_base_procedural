#include "includes/common.h"

SDL_Window    *init_sdl();
unsigned int  set_seed(int argc, char *argv[]);
void          clean(state_t *state, SDL_Window *window);

int           main(int argc, char *argv[]) {
  SDL_Window  *window;
  state_t     state;
  SDL_Event   event;
  int         quit;

  window = init_sdl();
  state.seed = set_seed(argc, argv);
  state.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  state.grid = NULL;
  state.rooms = NULL;
  init_texture(&state, "assets/Sprite-0002.bmp", 5, 1);
  init_level(&state, 1);
  level_into_grid(&state);
  quit = 0;  

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);

    /* Draw your graphics */

    if (DEBUG == 1) {
      draw_level(&state);
      draw_connections(&state);
    } else {
      draw_grid(&state);
    }
    
    /**********************/

    SDL_RenderPresent(state.renderer);
    SDL_Delay(16);
  }

  clean(&state, window);
  return 0;
}

SDL_Window    *init_sdl() {
  SDL_Window  *window;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL initialization failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Window creation failed: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  return window;
}

unsigned int  set_seed(int argc, char *argv[]) {
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
  return seed;
}

void          clean(state_t *state, SDL_Window *window) {
  free_level(state);
  free_texture(state);
  SDL_DestroyRenderer(state->renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
