#include "includes/common.h"

void          draw_light(state_t *state) {
  float       ray_x,ray_y,ray_angle,rad;
  int         angle,distance;
  coord_t     coord_ray;
  int         player_angle;
  SDL_Vertex  *vertices = NULL;
  size_t      num_vertices = 0;
  size_t      capacity = 0;

  rad = M_PI / 180;
  player_angle = direction_to_degrees(state->player->direction);
  coord_t p = { state->player->dst_screen.x - state->scroll.x, state->player->dst_screen.y - state->scroll.y };
  add_vertex(&vertices, &num_vertices, &capacity, p.x, p.y);

  // for (angle = player_angle - LIGHT_ANGLE / 2 ; angle < LIGHT_ANGLE / 2 ; angle++) {
  //   ray_angle = angle * rad;
  //   // cast ray in pixels
  //   for (distance = 0; distance < state->player->light ; distance++) {
  //     ray_x = state->player->dst_screen.x + distance * cos(ray_angle);
  //     ray_y = state->player->dst_screen.y + distance * sin(ray_angle);

  //     // switch to grid coords
  //     coord_ray.x = ray_x / state->tile_screen_size;
  //     coord_ray.y = ray_y / state->tile_screen_size;

  //     // check if is in room
  //     if (is_in_room(coord_ray, state->player->current_node->rect) != 0) {
  //       // DEBUG_MSG("");
  //       break;
  //     }

  //     // if we hit a corner of the room (light rect ?), we register the vertex
  //     if (is_corner_wall(state->player->current_node->rect, coord_ray.x, coord_ray.y) == 0) {
  //       add_vertex(&vertices, &num_vertices, &capacity, coord_ray.x * state->tile_screen_size, coord_ray.y * state->tile_screen_size);
  //       break;
  //     } else if (type_stops_light(state->grid[coord_ray.x][coord_ray.y]) == 0) {
  //       add_vertex(&vertices, &num_vertices, &capacity, coord_ray.x * state->tile_screen_size, coord_ray.y * state->tile_screen_size);
  //       break;
  //     }
  //   }
  // }
  SDL_SetTextureBlendMode(state->black_texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(state->renderer, state->black_texture);
  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
  SDL_RenderClear(state->renderer);
  // draw each vertices as triangle on the texture
  SDL_Rect rect;
  rect.x = state->player->dst_screen.x - state->scroll.x - state->tile_screen_size;
  rect.y = state->player->dst_screen.y - state->scroll.y - state->tile_screen_size;
  rect.w = state->tile_screen_size * 2;
  rect.h = state->tile_screen_size * 2;
  SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 100);
  SDL_RenderFillRect(state->renderer, &rect);
  // printf("player: %d, %d\n", rect.x, rect.y);
  
  // add_vertex(&vertices, &num_vertices, &capacity, p.x + state->tile_screen_size, p.y);
  // add_vertex(&vertices, &num_vertices, &capacity, p.x, p.y + state->tile_screen_size);
  printf("num_vertices: %ld\n", num_vertices);
  // for (size_t i = 0 ; i < num_vertices ; i++) {
  //   printf("vertex: %f, %f\n", vertices[i].position.x, vertices[i].position.y);
  // }
  // SDL_RenderGeometry(state->renderer, state->black_texture, vertices, 3, NULL, 0);
  // for (size_t i = 2; i < num_vertices ; i++) {
  //   int indices[3] = { i - 1, 0, i };
  //   SDL_RenderGeometry(state->renderer, state->black_texture, vertices, 3, indices, 3);
  // }

  free(vertices);
  SDL_SetRenderTarget(state->renderer, NULL);
}

void          add_vertex(SDL_Vertex **vertices, size_t *num_vertices, size_t *capacity, int x, int y) {
  if (*num_vertices >= *capacity) {
    *capacity += 10;
    SDL_Vertex *tmp = realloc(*vertices, *capacity * sizeof(SDL_Vertex));
    if (tmp == NULL) {
      DEBUG_MSG("failed vertex realloc");
      exit(EXIT_FAILURE);
    }
    *vertices = tmp;
  }

  (*vertices)[*num_vertices].position.x = (float)x;
  (*vertices)[*num_vertices].position.y = (float)y;
  (*vertices)[*num_vertices].color.r = 0;
  (*vertices)[*num_vertices].color.g = 0;
  (*vertices)[*num_vertices].color.b = 0;
  (*vertices)[*num_vertices].color.a = 100;
  (*vertices)[*num_vertices].tex_coord.x = 1;
  (*vertices)[*num_vertices].tex_coord.y = 1;
  (*num_vertices)++;
}

// in which direction goes the angle ?
// replace wall vertice with intersection vertice
// replace inner wall corner with outer wall corner

// even for corners of room we'r gonna need to check 3 edges :((
