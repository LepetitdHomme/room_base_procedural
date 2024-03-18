#ifndef TEXTURES_H
#define TEXTURES_H

#include "structs.h"

/*                      textures */
void                    init_texture(SDL_Renderer *renderer, texture_t *destination, const char *path, int num_x, int num_y);
void          					init_image(SDL_Renderer *renderer, texture_t *destination, const char *path);
void                    free_texture(texture_t *texture);
void          					free_texture_image(texture_t *texture);
SDL_Rect                grid_value_to_tileset_rect(state_t *state, int x);

#endif
