#include "includes/common.h"

void          init_texture(SDL_Renderer *renderer, texture_t *destination, const char *path, int num_x, int num_y) {
  SDL_Surface   *tmp = NULL;
  SDL_Rect      rect;

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
}

void          free_texture(texture_t *texture) {
  if (texture != NULL) {
    SDL_DestroyTexture(texture->texture);
  }
  free(texture);
}

/*            TODO: BRUT VALUES */
SDL_Rect      grid_value_to_tileset_rect(state_t *state, int x) {
  SDL_Rect src;

  switch (x) {
    case EMPTY:
      src.x = 0;;
      src.y = 0;
      break;
    case WALL_UP:
    case WALL_DOWN:
    case WALL_LEFT:
    case WALL_RIGHT:
      src.x = 2 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case CORNER_TOP_LEFT:
    case CORNER_TOP_RIGHT:
    case CORNER_BOT_LEFT:
    case CORNER_BOT_RIGHT:
      src.x = 4 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case FLOOR:
    case CORRIDOR:
      src.x = 1 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case DOOR_SRC:
    case DOOR_UP:
    case DOOR_DOWN:
    case DOOR_LEFT:
    case DOOR_RIGHT:
      src.x = 3 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case DOOR_DST:
      src.x = 3 * state->level_texture->tile_w;
      src.y = 0;
      break;
    default:
      src.x = 0 * state->level_texture->tile_w;
      src.y = 0;
  }
  src.w = state->level_texture->tile_w;
  src.h = state->level_texture->tile_h;
  return src;
}