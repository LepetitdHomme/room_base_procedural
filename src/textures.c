#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include "../include/textures.h"
#include "../include/macros.h"

void          init_texture(SDL_Renderer *renderer, texture_t *destination, const char *path, int num_x, int num_y) {
  SDL_Surface   *tmp = NULL;
  SDL_Rect      rect;
  int           i,j,numtile;

  if (destination == NULL) {
    DEBUG_MSG("destination texture NULL");
    exit(EXIT_FAILURE);
  }

  // We create a surface
  if ((tmp = SDL_LoadBMP(path)) == NULL) {
    DEBUG_MSG("SDL_LoadBMP Failed");
    exit(EXIT_FAILURE);
  }

  // we select the transparent color // sdl bmp cannot handle transparency, so we set a custom 'green screen': 255 0 255 magenta
  SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));

  // We create a texture from the surface
  if ((destination->texture = SDL_CreateTextureFromSurface(renderer, tmp)) == NULL) {
    DEBUG_MSG("SDL_Texture from surface Failed");
    exit(EXIT_FAILURE);
  }
  SDL_FreeSurface(tmp);

  // brut values
  destination->num_tiles_x = num_x;
  destination->num_tiles_y = num_y;

  SDL_QueryTexture(destination->texture, NULL, NULL, &rect.w, &rect.h);
  destination->tile_w = rect.w / num_x;
  destination->tile_h = rect.h / num_y;
  if ((destination->props = (tile_prop_t *)malloc(sizeof(tile_prop_t) * num_x * num_y)) == NULL) {
    DEBUG_MSG("destination tile prop malloc failure");
    exit(EXIT_FAILURE);
  }
  for (j = 0, numtile = 0 ; j < num_y ; j++) {
    for(i = 0 ; i < num_x ; i++, numtile++) {
      destination->props[numtile].src.x = i * destination->tile_w;
      destination->props[numtile].src.y = j * destination->tile_h;
      destination->props[numtile].src.w = destination->tile_w;
      destination->props[numtile].src.h = destination->tile_h;
    }
  }

  // printf("texture: numx: %d, numy: %d, w: %d, h: %d\n", num_x, num_y, destination->tile_w, destination->tile_h);
}

void          init_image(SDL_Renderer *renderer, texture_t *destination, const char *path) {
  SDL_Surface   *tmp = NULL;
  SDL_Rect      rect;

  if (destination == NULL) {
    DEBUG_MSG("destination image NULL");
    exit(EXIT_FAILURE);
  }

  // We create a surface
  if ((tmp = IMG_Load(path)) == NULL) {
    DEBUG_MSG("IMG_Load Failed");
    exit(EXIT_FAILURE);
  }

  // we select the transparent color // sdl bmp cannot handle transparency, so we set a custom 'green screen': 255 0 255 magenta
  // SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 255, 0, 255));

  // We create a texture from the surface
  if ((destination->texture = SDL_CreateTextureFromSurface(renderer, tmp)) == NULL) {
    DEBUG_MSG("SDL_Texture from surface Failed");
    exit(EXIT_FAILURE);
  }
  SDL_FreeSurface(tmp);

  // Uint8 alpha = 0x7F;
  // Sets the alpha into the texture.
  // SDL_SetTextureAlphaMod(destination->texture, alpha);

  SDL_QueryTexture(destination->texture, NULL, NULL, &rect.w, &rect.h);
  destination->tile_w = rect.w;
  destination->tile_h = rect.h;

  destination->num_tiles_x = -1;
  destination->num_tiles_y = -1;
  destination->props = NULL;
}

void          free_texture_image(texture_t *texture) {
  if (texture != NULL) {
    SDL_DestroyTexture(texture->texture);
  }
  free(texture);
}

void          free_texture(texture_t *texture) {
  if (texture != NULL) {
    SDL_DestroyTexture(texture->texture);
  }
  free(texture->props);
  free(texture);
}

/*            TODO: BRUT VALUES */
SDL_Rect      grid_value_to_tileset_rect(state_t *state, int x) {
  SDL_Rect src;

  switch (x) {
    case EMPTY:
      src.x = 0;
      src.y = 0;
      break;
    case CORNER_TOP_LEFT:
    case CORNER_TOP_RIGHT:
      src.x = 1 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case WALL_UP:
      src.x = 2 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case WALL_LEFT:
    case DOOR_LEFT:
    case WALL_RIGHT:
    case DOOR_RIGHT:
      src.x = 3 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case DOOR_UP:
      src.x = 4 * state->level_texture->tile_w;
      src.y = 1 * state->level_texture->tile_h;
      break;
    case DOOR_DOWN:
      src.x = 5 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case CORNER_BOT_LEFT:
    case CORNER_BOT_RIGHT:
      src.x = 6 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case WALL_DOWN:
      src.x = 7 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case FLOOR:
    case CORRIDOR:
      src.x = 8 * state->level_texture->tile_w;
      src.y = 0;
      break;
    default:
      src.x = 0;
      src.y = 0;
  }
  src.w = state->level_texture->tile_w;
  src.h = state->level_texture->tile_h;
  return src;
}