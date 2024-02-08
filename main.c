#include "includes/common.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv; /* signature necessary for windows, but -wall raises unused variables */

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

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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

    /* Draw your graphics here (currently an empty black window) */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    /* SDL_Delay(16); Uncomment to limit frame rate (approximately 60 FPS) */
  }

  /* Clean up */
  SDL_DestroyWindow(window);
  SDL_Quit();

  /* Marking variables as unused */
  (void)quit;  /* Use (void) to explicitly indicate that the variable is intentionally unused */

  printf("SDL2 application closed\n");

  return 0;
}
