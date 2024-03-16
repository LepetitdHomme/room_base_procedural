#ifndef PLAYER_H
#define PLAYER_H

#include "structs.h"

/*                      player */
void                    init_player(state_t *state);
void                    free_player(state_t *state);
void                    player_reset_screen_from_grid(state_t *state);
void                    player_update_node(player_t *player);
void                    player_update_direction(player_t *player);
void                    player_move_proceed(state_t *state, SDL_Rect test);
void                    player_move_to_door(state_t *state, SDL_Rect dest);
void                    player_refine_move_attempt(state_t *state, int dx, int dy);
void                    player_move(state_t *state, int dx, int dy);
int                     player_move_attempt(state_t *state, int dx, int dy);

#endif
