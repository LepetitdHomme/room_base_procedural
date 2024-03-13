#include "includes/common.h"

/*
*     Gets a grid coord and computes the 3 vertices on which we
*       want to display light.
*       => anti clockwise
*/
triangle_t    screen_triangle(state_t *state, coord_t ray, enum Octant octant) {
  triangle_t  triangle;


  if (octant == NW) {
    // top right vertex of corner
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y;
    // top left vertex of corner
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y;
    // bottom left vertex of corner
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
  } else if (octant == NE) {
    // bot right vertex of corner
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    // top right vertex of corner
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y;
    // top left vertex of corner
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y;
  } else if (octant == SW) {
    // top left vertex of corner
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y;
    // bot left vertex of corner
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    // bottom right vertex of corner
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
  } else if (octant == SE) {
    // bot left vertex of corner
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    // bot right vertex of corner
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    // top right vertex of corner
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y;
  } else if (octant == E) {//anticlockwise! we take the back of the cell to put light on wall
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y;
  } else if (octant == N) {
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y;
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y ;
  } else if (octant == W) {
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y;
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
  } else { // S-SUD
    triangle.a.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.a.y = ray.y * state->tile_screen_size - state->scroll.y;
    triangle.b.x = ray.x * state->tile_screen_size - state->scroll.x;
    triangle.b.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
    triangle.c.x = ray.x * state->tile_screen_size - state->scroll.x + state->tile_screen_size - 1;
    triangle.c.y = ray.y * state->tile_screen_size - state->scroll.y + state->tile_screen_size - 1;
  }

  return triangle;
}

void          draw_light(state_t *state) {
  float           ray_x,ray_y,ray_angle;
  int             angle,distance,opacity;
  coord_t         coord_ray,p;
  int             player_angle;
  SDL_Vertex      *vertices = NULL;
  SDL_Rect        light_map;
  size_t          num_vertices = 0;
  size_t          capacity = 0;
  enum Octant     octant;
  triangle_t      triangle;
  coord_t         intersection;

  player_angle = direction_to_degrees(state->player->direction);
  light_map.x = state->player->current_node->rect.x;
  light_map.y = state->player->current_node->rect.y;
  light_map.w = state->player->current_node->rect.w;
  light_map.h = state->player->current_node->rect.h;
  p.x = state->player->dst_screen.x - state->scroll.x;
  p.y = state->player->dst_screen.y - state->scroll.y;
  add_vertex(&vertices, &num_vertices, &capacity, p.x, p.y, 255);

  for (angle = LIGHT_ANGLE / 2 ; angle >= player_angle - LIGHT_ANGLE / 2 ; angle--) {
    ray_angle = angle * RADIAN;
    // cast ray in pixels
    for (distance = 0; distance < state->player->light ; distance++) {
      intersection.x = -1;
      ray_x = state->player->dst_screen.x + distance * cos(ray_angle);
      ray_y = state->player->dst_screen.y + distance * sin(ray_angle);

      // switch to grid coords
      coord_ray.x = ray_x / state->tile_screen_size;
      coord_ray.y = ray_y / state->tile_screen_size;

      // check if is in room
      if (is_in_room(coord_ray, state->player->current_node->rect) != 0) break;

      opacity = compute_transparency(distance, state->player->light);

      if (type_stops_light(state->grid[coord_ray.x][coord_ray.y]) == 0) {
        if (state->player->current_node->light_map[coord_ray.x - light_map.x][coord_ray.y - light_map.y] == 1) {
          break;
        }
        octant = wall_to_octant(state->player->pos, coord_ray);
        // printf("octant: %d\n", octant);
        triangle = screen_triangle(state, coord_ray, octant);
        // find intersection to wall with each found screen corner of grid corner
        // triangle 1
        intersection = find_intersection_with_walls(state, p, triangle.a, octant);
        if (intersection.x != -1) {
          add_vertex(&vertices, &num_vertices, &capacity, intersection.x, intersection.y, 255);
          add_vertex(&vertices, &num_vertices, &capacity, triangle.a.x, triangle.a.y, 255);
        }
        
        // triangle 2
        intersection = find_intersection_with_walls(state, p, triangle.b, octant);
        if (intersection.x != -1) {
          if (triangle.c.x == -1) {//if wall is lighten up in 3 corners, we dont join the middle one
            add_vertex(&vertices, &num_vertices, &capacity, triangle.b.x, triangle.b.y, 255);
            add_vertex(&vertices, &num_vertices, &capacity, intersection.x, intersection.y, 255);
          } else {
            add_vertex(&vertices, &num_vertices, &capacity, triangle.b.x, triangle.b.y, 255);
          }
        }

        // triangle 3
        if (triangle.c.x != -1) {
          intersection = find_intersection_with_walls(state, p, triangle.c, octant);
          if (intersection.x != -1) {
            // on third corner we put intersection last, cuz the order should be the ray cast angle (anticlockwise)
            add_vertex(&vertices, &num_vertices, &capacity, triangle.c.x, triangle.c.y, 255);
            add_vertex(&vertices, &num_vertices, &capacity, intersection.x, intersection.y, 255);
          } else {
            DEBUG_MSG("");
          }
        }
        state->player->current_node->light_map[coord_ray.x - light_map.x][coord_ray.y - light_map.y] = 1;
        break;
      } else if (is_corner_wall(state->player->current_node->rect, coord_ray.x, coord_ray.y) == 0) {
        if (state->player->current_node->light_map[coord_ray.x - light_map.x][coord_ray.y - light_map.y] == 1) {
          break;
        }
        octant = corner_to_octant(state->grid[coord_ray.x][coord_ray.y]);
        triangle = screen_triangle(state, coord_ray, octant);
        add_vertex(&vertices, &num_vertices, &capacity, triangle.a.x, triangle.a.y, 255);
        add_vertex(&vertices, &num_vertices, &capacity, triangle.b.x, triangle.b.y, 255);
        add_vertex(&vertices, &num_vertices, &capacity, triangle.c.x, triangle.c.y, 255);
        state->player->current_node->light_map[coord_ray.x - light_map.x][coord_ray.y - light_map.y] = 1;
        break;
      }
    }
  }

  SDL_SetTextureBlendMode(state->black_texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(state->renderer, state->black_texture);
  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
  SDL_RenderClear(state->renderer);
  size_t num_triangles = num_vertices - 2 + 1;
  size_t num_indices = num_triangles * 3; // Each triangle has 3 vertices
  int *indices = (int *)malloc(num_indices * sizeof(int)); // ensure alloc OK

  // Construct the indices array
  size_t index = 0;
  for (size_t i = 2; i < num_vertices; i++) {
    indices[index++] = 0;
    indices[index++] = i - 1;
    indices[index++] = i;
  }
  indices[index++] = num_vertices - 1;
  indices[index++] = 0;
  indices[index++] = 1;
  // printf("num_triangles: %ld\n", num_triangles);
  // int j=0;
  // for (int i = 0 ; i < num_indices ; i++) {
  //   if (j % 3 == 0)
  //     printf("\n");
  //   printf("%d - ", indices[i]);

  //   j++;
  // }
  // printf("\n");
  SDL_RenderGeometry(state->renderer, NULL, vertices, num_vertices, indices, num_indices);

  free(indices);
  free(vertices);
  SDL_SetRenderTarget(state->renderer, NULL);
}

void          add_vertex(SDL_Vertex **vertices, size_t *num_vertices, size_t *capacity, int x, int y, int opacity) {
  // static int r = 0;
  // if (*num_vertices == 0) {
  //   r = 0;
  // }
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
  (*vertices)[*num_vertices].color.a = 255 - opacity;
  (*vertices)[*num_vertices].tex_coord.x = 1;
  (*vertices)[*num_vertices].tex_coord.y = 1;
  (*num_vertices)++;
  // r+=20;
}

int           compute_transparency(int distance, int light) {
  if (distance <= 0) {
    return 255; // Full opacity when distance is 0 or negative
  } else if (distance >= light) {
    return 0;   // No opacity when distance is equal to or greater than light
  } else {
    // Linear interpolation to compute opacity
    // Opacity varies from 0 (at distance 0) to 255 (at distance 'light')
    return (int)(255.0 * (1.0 - ((double)distance / light)));
  }
}

// in which direction goes the angle ?
// replace wall vertice with intersection vertice
// replace inner wall corner with outer wall corner

// even for corners of room we'r gonna need to check 3 edges :((
