#include "includes/common.h"

void 					inputs_keydown(state_t *state, SDL_Event *event, int *inputs_state) {
	if (event->key.keysym.sym == SDLK_w) {
		if (state->player->delta_y >= -state->player->speed) {
			inputs_state[3] = 1;
			state->player->delta_y = -state->player->speed;
		}
	}
	if (event->key.keysym.sym == SDLK_s) {
		if (state->player->delta_y <= state->player->speed) {
			inputs_state[1] = 1;
			state->player->delta_y = state->player->speed;
		}
	}
	if (event->key.keysym.sym == SDLK_a) {
		if (state->player->delta_x >= -state->player->speed) {
			inputs_state[0] = 1;
			state->player->delta_x = -state->player->speed;
		}
	}
	if (event->key.keysym.sym == SDLK_d) {
		if (state->player->delta_x <= state->player->speed) {
			inputs_state[2] = 1;
			state->player->delta_x = state->player->speed;
		}
	}
}

/* inputs_state allows for multiple key press without having to perma call SDL get keyboard state */
void 					inputs_keyup(state_t *state, SDL_Event *event, int *inputs_state) {
	if (event->key.keysym.sym == SDLK_w) {
		inputs_state[3] = 0;
		if (inputs_state[1] == 1) {
			state->player->delta_y = state->player->speed;
		} else {
			state->player->delta_y = 0;
		}
	}
	if (event->key.keysym.sym == SDLK_s) {
		inputs_state[1] = 0;
		if (inputs_state[3] == 1) {
			state->player->delta_y = -state->player->speed;
		} else {
			state->player->delta_y = 0;
		}
	}
	if (event->key.keysym.sym == SDLK_a) {
		inputs_state[0] = 0;
		if (inputs_state[2] == 1) {
			state->player->delta_x = state->player->speed;
		} else {
			state->player->delta_x = 0;
		}
	}
	if (event->key.keysym.sym == SDLK_d) {
		inputs_state[2] = 0;
		if (inputs_state[0] == 1) {
			state->player->delta_x = -state->player->speed;
		} else {
			state->player->delta_x = 0;
		}
	}
}

void          inputs_wheel(state_t *state, SDL_Event *event) {
  if (event->type == SDL_MOUSEWHEEL) {
    if (event->wheel.y > 0) {
      state->zoom.x = clamp(state->zoom.x - 2, 10, 20);
      compute_screen_sizes(state);
    } else if (event->wheel.y < 0) {
      state->zoom.x = clamp(state->zoom.x + 2, 10, 20);
      compute_screen_sizes(state);
    }
  }
}

void          inputs_handle(state_t *state, int *quit, SDL_Event *event, int *inputs_state) {
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
			*quit = 1;
			return;
		}
		if (event->type == SDL_QUIT) {
			*quit = 1;
			return;
		}

		switch (event->type) {
			case SDL_KEYDOWN:
				inputs_keydown(state, event, inputs_state);
				break;
			case SDL_KEYUP:
				inputs_keyup(state, event, inputs_state);
				break;
      case SDL_MOUSEWHEEL:
        inputs_wheel(state, event);
			default:
				break;
		}
	}
}
