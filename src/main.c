#include <stdlib.h>// exit/exitfailure/malloc
#include <unistd.h>// read/write
#include <time.h>
#include "../include/main.h"
#include "../include/macros.h"// stdio/math
#include "../include/textures.h"
#include "../include/level.h"
#include "../include/inputs.h"
#include "../include/player.h"
#include "../include/grid.h"
#include "../include/graph.h"
#include "../include/connections.h"
#include "../include/shadow_casting.h"
#include "../include/draw.h"
// #include <stdio.h>
// #include <stdbool.h>

int           main(int argc, char *argv[]) {
  SDL_Window  *window;
  SDL_Event   event;
  state_t     state;
  int         quit;

  window = init_sdl();
  // We initiate the main state structure
  state.seed = set_seed(argc, argv);
  state.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  state.grid = NULL;
  state.rooms = NULL;
  state.connections = NULL;
  state.num_connections = 0;
  state.graph = NULL;
  state.zoom.x = -1;
  state.display_map = 0;
  state.player = NULL;
  state.level_texture = (texture_t *)malloc(sizeof(texture_t));
  if (state.level_texture == NULL) {
    DEBUG_MSG("level texture malloc failed");
    exit(EXIT_FAILURE);
  }
  init_texture(state.renderer, state.level_texture, "assets/Sprite-0006 Copy.bmp", 9, 2);
  state.black_texture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
  if (state.black_texture == NULL) {
    printf("Black texture could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  // We compute the first level
  init_level(&state, 1);
  level_to_grid(&state, state.graph);
  draw_compute_screen_sizes(&state);

  // We prepare the main loop
  quit = 0;
  int inputs_state[4] = { 0, 0, 0, 0 }; // awsd :/
  SDL_Rect window_rect = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

  while (!quit) {
    state.ticks = SDL_GetTicks();

    inputs_handle(&state, &quit, &event, &inputs_state[0]);
    player_move(&state, state.player->delta_x, state.player->delta_y);
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);

    /* Draw your graphics */
    draw_node(&state);
    draw_entities(&state);
    perform_shadow_casting(&state);
    SDL_RenderCopy(state.renderer, state.black_texture, NULL, &window_rect);

    if (state.display_map == 1) {
      if (DEBUG_GRID) {
        draw_grid(&state);
      } else {
        draw_map_grid(&state);
      }
    }

    SDL_RenderPresent(state.renderer);
    SDL_Delay(16);
  }

  printf("SEED: %d\n", state.seed);
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
  free_grid(state);
  free_graph(state);
  free_connections(state);
  free_texture(state->level_texture);
  SDL_DestroyRenderer(state->renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
