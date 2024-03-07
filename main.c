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
  init_texture(state.renderer, state.level_texture, "assets/Sprite-0003.bmp", 5, 1);
  init_level(&state, 2);
  level_to_grid(&state, state.graph);
  draw_compute_screen_sizes(&state);
  quit = 0;
  // TODO: do something bout this
  int inputs_state[4] = { 0, 0, 0, 0 }; // awsd :/

  while (!quit) {
    state.ticks = SDL_GetTicks();

    inputs_handle(&state, &quit, &event, &inputs_state[0]);
    player_move(&state, state.player->delta_x, state.player->delta_y);
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);

    /* Draw your graphics */

    draw_node(&state);
    draw_entities(&state);
    if (state.display_map == 1) {
      if (DEBUG_GRID) {
        draw_grid(&state);
      } else {
        draw_map_grid(&state);
      }
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
  free_grid(state);
  free_graph(state);
  free_connections(state);
  free_texture(state->level_texture);
  SDL_DestroyRenderer(state->renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
