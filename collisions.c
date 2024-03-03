#include "includes/common.h"

int 					is_colliding(state_t *state, SDL_Rect dst) {
	int 				xmin;
	int 				xmax;
	int 				ymin;
	int 				ymax;
	int 				screen_to_grid_x;
	int 				screen_to_grid_y;

	screen_to_grid_x = state->grid_w * state->tile_screen_size;
	screen_to_grid_y = state->grid_h * state->tile_screen_size;

	if (dst.x < 0 || (dst.x + dst.w - 1) >= (screen_to_grid_x)) {
		return 1;
	} else if (dst.y < 0 || (dst.y + dst.h -1) >= (screen_to_grid_y)) {
		return 1;
	}

	xmin = dst.x / state->tile_screen_size;
	xmax = (dst.x + dst.w - 1) / state->tile_screen_size;

	ymin = (dst.y + dst.h / 2) / state->tile_screen_size; // collide with feet only
	ymax = (dst.y + dst.h - 1) / state->tile_screen_size;

	for (int i = xmin ; i <= xmax ; i++) {
		for (int j = ymin ; j <= ymax ; j++) {
			// printf("collision: %d, %d\n", i, j);
			if (state->grid[i][j] != FLOOR) {
				return 1;
			}
		}
	}
	return 0;
}
