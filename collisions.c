#include "includes/common.h"

enum Type     collisions_priority(enum Type collision1, enum Type collision2) {
  if (collision1 == FLOOR) {
    // if (DEBUG_COLLISIONS) DEBUG_MSG("FLOOR VS 2");
    return collision2;
  }
  if (is_door_type(collision1) == 0) {
    if (collision2 != FLOOR) {
      if (DEBUG_COLLISIONS) DEBUG_MSG("DOOR VS NOT FLOOR");
      return collision2;
    } else {
      if (DEBUG_COLLISIONS) DEBUG_MSG("DOOR VS FLOOR");
      return collision1;
    }
  }
  if (DEBUG_COLLISIONS) DEBUG_MSG("WALL");
  return collision1;
}

enum Type     is_colliding_with(state_t *state, SDL_Rect dst) {
	int 				xmin;
	int 				xmax;
	int 				ymin;
	int 				ymax;
	int 				screen_to_grid_x;
	int 				screen_to_grid_y;
  enum Type   prioritized_collision;

	screen_to_grid_x = state->grid_w * state->tile_screen_size;
	screen_to_grid_y = state->grid_h * state->tile_screen_size;

	if (dst.x < 0 || (dst.x + dst.w - 1) >= (screen_to_grid_x)) {
		return 1;
	} else if (dst.y < 0 || (dst.y + dst.h -1) >= (screen_to_grid_y)) {
		return 1;
	}
	xmin = dst.x / state->tile_screen_size;
	xmax = (dst.x + dst.w - 1) / state->tile_screen_size;

	ymin = dst.y / state->tile_screen_size;
	ymax = (dst.y + dst.h - 1) / state->tile_screen_size;

  prioritized_collision = FLOOR;

  /* remember, we are checking the move ATTEMPT = destination, here. */
	for (int i = xmin ; i <= xmax ; i++) {
		for (int j = ymin ; j <= ymax ; j++) {
      prioritized_collision = collisions_priority(prioritized_collision, state->grid[i][j]);
		}
	}
  if (DEBUG_COLLISIONS) printf("collision prioritized = %d\n", prioritized_collision);
	return prioritized_collision;
}
