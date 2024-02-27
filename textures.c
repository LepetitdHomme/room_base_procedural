#include "includes/common.h"

void    init_texture(state_t *state, const char *path, int num_x, int num_y) {
  SDL_Surface *tmp = NULL;
  SDL_Rect rect;

  // We create a surface
  tmp = SDL_LoadBMP(path);
  if (tmp == NULL) {
    DEBUG_MSG("SDL_LoadBMP Failed");
    exit(EXIT_FAILURE);
  }
  SDL_SetColorKey(tmp, SDL_TRUE, SDL_MapRGB(tmp->format, 25, 225, 115));

  // We malloc state->level_texture
  state->level_texture = (texture_t *)malloc(sizeof(texture_t));
  if (state->level_texture == NULL) {
    DEBUG_MSG("level texture malloc failed");
    exit(EXIT_FAILURE);
  }

  // We create a texture from the surface
  state->level_texture->texture = SDL_CreateTextureFromSurface(state->renderer, tmp);
  if (state->level_texture->texture == NULL) {
    DEBUG_MSG("SDL_Texture from surface Failed");
    exit(EXIT_FAILURE);
  }
  SDL_FreeSurface(tmp);

  state->level_texture->num_tiles_x = num_x;
  state->level_texture->num_tiles_y = num_y;

  SDL_QueryTexture(state->level_texture->texture, NULL, NULL, &rect.w, &rect.h);
  state->level_texture->tile_w = rect.w / num_x;
  state->level_texture->tile_h = rect.h / num_y;
  state->scroll.x = 0;
  state->scroll.y = 0;//8 * state->TILE_HEIGHT;
  // define limit scrolling
  state->limit_scroll.x = 100;
  state->limit_scroll.y = 100;
  state->limit_scroll.w = 100;
  state->limit_scroll.h = 100;
}

// BRUT VALUES
SDL_Rect      grid_value_to_tileset_rect(state_t *state, int x) {
  SDL_Rect src;

  switch (x) {
    case EMPTY:
      src.x = 5 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case WALL_UP:
    case WALL_DOWN:
    case WALL_LEFT:
    case WALL_RIGHT:
      src.x = 1 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case CORNER_TOP_LEFT:
    case CORNER_TOP_RIGHT:
    case CORNER_BOT_LEFT:
    case CORNER_BOT_RIGHT:
      src.x = 0;
      src.y = 0;
      break;
    case FLOOR:
      src.x = 1 * state->level_texture->tile_w;
      src.y = 1 * state->level_texture->tile_h;
      break;
    case DOOR_SRC:
      src.x = 5 * state->level_texture->tile_w;
      src.y = 0;
      break;
    case DOOR_DST:
      src.x = 5 * state->level_texture->tile_w;
      src.y = 0;
      break;
    default:
      src.x = 5 * state->level_texture->tile_w;
      src.y = 0;
  }
  src.w = state->level_texture->tile_w;
  src.h = state->level_texture->tile_h;
  return src;
}