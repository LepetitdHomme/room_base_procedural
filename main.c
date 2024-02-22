#include "includes/common.h"

int main() {
  srand(time(NULL));
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
  // init_grid(&state);
  init_level(&state, 1); 

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

    draw_grid(&state);
    SDL_RenderPresent(state.renderer);

    // SDL_Delay(30);
  }

  /* Clean up */
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
