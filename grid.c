#include "includes/common.h"

void          init_grid(state_t *state, int complexity) {
  state->scale = SCALE;

  state->grid_w = state->scale * complexity;
  state->grid_h = state->scale * complexity;
  state->grid = (int **)malloc(sizeof(int *) * state->grid_w);

  for (int i = 0 ; i < state->grid_w ; i++) {
    state->grid[i] = (int *)malloc(sizeof(int) * state->grid_h);
  }

  for(int i = 0; i < state->grid_w ; i++) {
    for(int j = 0; j < state->grid_h ; j++) {
      state->grid[i][j] = 0;
    }
  }
}

void          free_grid(state_t *state) {
  for(int i = 0; i < state->grid_w ; i++) {
    free(state->grid[i]);
    state->grid[i] = NULL;
  }
  free(state->grid);
  state->grid = NULL;
}

SDL_Color     pick_color(state_t *state, int i, int j) {
  SDL_Color color;

  switch (state->grid[i][j]) {
    case EMPTY://empty
      color.r = 0;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    case WALL_UP:
    case WALL_DOWN:
    case WALL_LEFT:
    case WALL_RIGHT:
      color.r = 127;
      color.g = 127;
      color.b = 127;
      color.a = 127;
      break;
    case FLOOR://floor
      color.r = 255;
      color.g = 255;
      color.b = 255;
      color.a = 255;
      break;
    case DOOR_SRC://door
      color.r = 0;
      color.g = 255;
      color.b = 0;
      color.a = 255;
      break;
    case DOOR_DST:
      color.r = 255;
      color.g = 0;
      color.b = 0;
      color.a = 255;
      break;
    default:
      color.r = 255;
      color.g = 255;
      color.b = 255;
      color.a = 255;
  }

  return color;
}  

void          draw_connections(state_t *state) {
  room_t *tmp = state->rooms;
  door_t *tmp_d = NULL;

  int scale_x,scale_y;
  scale_x = WINDOW_WIDTH / state->grid_w;
  scale_y = WINDOW_HEIGHT / state->grid_h;

  while (tmp) {
    tmp_d = tmp->doors;
    while (tmp_d) {
      coord_t a,b;
      a.x = tmp->center.x * scale_x;
      a.y = tmp->center.y * scale_y;
      b.x = tmp_d->room->center.x * scale_x;
      b.y = tmp_d->room->center.y * scale_y;
      SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
      SDL_RenderDrawLine(state->renderer, a.x, a.y, b.x, b.y);
      tmp_d = tmp_d->next;
    }
    tmp = tmp->next;
  }
}

void          draw_level(state_t *state) {
  SDL_Rect rect;
  SDL_Color color;

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

  int scale_x,scale_y;
  scale_x = WINDOW_WIDTH / state->grid_w;
  scale_y = WINDOW_HEIGHT / state->grid_h;

  for (int i = 0; i < state->grid_w ; i++) {
    for (int j = 0 ; j < state->grid_h ; j++) {
      rect.x = i * scale_x;
      rect.y = j * scale_y;
      rect.w = scale_x;
      rect.h = scale_y;

      color = pick_color(state, i, j);
      SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, color.a);
      SDL_RenderFillRect(state->renderer, &rect);
    }
  }
}

double        angle_from_type(enum Type type) {
  double angle;

  switch (type) {
    case EMPTY:
      angle = 0.0;
      break;
    case WALL_UP:
      angle = 0.0;
      break;
    case WALL_DOWN:
      angle = 180.0;
      break;
    case WALL_LEFT:
      angle = -90.0;
      break;
    case WALL_RIGHT:
      angle = 90.0;
      break;
    case CORNER_TOP_LEFT:
      angle = 0.0;
      break;
    case CORNER_TOP_RIGHT:
      angle = 90.0;
      break;
    case CORNER_BOT_LEFT:
      angle = -90.0;
      break;
    case CORNER_BOT_RIGHT:
      angle = 180.0;// -180.0;
      break;
    default:
      angle = 0.0;
  }
  return angle;
}

void          draw_grid(state_t *state) {
  SDL_Rect          src,dst;
  SDL_Color         color;
  enum Type         tile_type;
  SDL_RendererFlip  flip = SDL_FLIP_NONE;
  room_t            *room = state->player->room;
  //                WINDOW RATIO should ensure cell_h maintains aspect ratio;
  //                + 2 is for around the room;
  float             cell_size_float = (float)WINDOW_WIDTH / (room->room.w + 2);
  int               cell_size = (int)cell_size_float;
  SDL_Point         center = {cell_size / 2, cell_size / 2};

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);

  for (int i = room->room.x - 1; i < room->room.x + room->room.w + 1; i++) {
    for (int j = room->room.y - 1 ; j < room->room.y + room->room.h + 1; j++) {

      if (i < room->room.x || i >= room->room.x + room->room.w || j < room->room.y || j >= room->room.y + room->room.h) {
        src = grid_value_to_tileset_rect(state, EMPTY);
        tile_type = EMPTY;
      } else {
        src = grid_value_to_tileset_rect(state, state->grid[i][j]);
        tile_type = state->grid[i][j];
      }

      dst.x = (i - (room->room.x - 1)) * cell_size_float;
      dst.y = (j - (room->room.y - 1)) * cell_size_float;
      dst.w = cell_size;
      dst.h = cell_size;
      SDL_RenderCopyEx(state->renderer, state->level_texture->texture, &src, &dst, angle_from_type(tile_type), &center, flip);
    }
  }
}
