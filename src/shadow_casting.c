#include <stdlib.h>
#include "../include/macros.h"
#include "../include/type.h"
#include "../include/shadow_casting.h"

void          add_vertex(light_edge **vertices, size_t *num_vertices, size_t *capacity, int x, int y, int opacity, float angle) {
  int         first = 0;

  // starts with a dirty trick to force player pos to stay on top of qsort
  if (*capacity == 0) {
    first = 1;
  }
  if (*num_vertices >= *capacity) {
    *capacity += 10;
    light_edge *tmp = realloc(*vertices, *capacity * sizeof(light_edge));
    if (tmp == NULL) {
      DEBUG_MSG("failed vertex realloc");
      exit(EXIT_FAILURE);
    }
    *vertices = tmp;
  }

  (*vertices)[*num_vertices].a.position.x = (float)x;
  (*vertices)[*num_vertices].a.position.y = (float)y;
  (*vertices)[*num_vertices].a.color.r = 0;
  (*vertices)[*num_vertices].a.color.g = 0;
  (*vertices)[*num_vertices].a.color.b = 0;
  (*vertices)[*num_vertices].a.color.a = 255 - opacity;
  (*vertices)[*num_vertices].a.tex_coord.x = 1;
  (*vertices)[*num_vertices].a.tex_coord.y = 1;
  (*vertices)[*num_vertices].angle_a = angle;
  if (first == 1) {
    (*vertices)[*num_vertices].angle_b = 1;
  } else {
    (*vertices)[*num_vertices].angle_b = 0;
  }
  (*num_vertices)++;
}

int           init_edges(state_t *state, light_edge **edges) {
  int           num_blocking_cells;
  int           num_edge,num_edge_tmp;
  SDL_Vertex    corner_1, corner_2, corner_3, corner_4;
  SDL_Rect      room;
  coord_t       cell_coord;
  enum Octant   octant;

  room = state->player->current_node->rect;

  num_blocking_cells = 0;
  for (int i = room.x ; i < room.x + room.w ; i++) {
    for (int j = room.y ; j < room.y + room.h ; j++) {
      if (type_stops_light(state->grid[i][j]) == 0) {
        num_blocking_cells++;
      }
    }
  }

  num_edge = 4 + num_blocking_cells * 4;
  // 4 is four edges per cell/block
  *edges = (light_edge *)malloc(sizeof(light_edge) * num_edge);
  if (*edges == NULL) {
    DEBUG_MSG("malloc failed for light edges");
    exit(EXIT_FAILURE);
  }

  num_edge_tmp = 0;
  for (int i = room.x ; i < room.x + room.w ; i++) {
    for (int j = room.y ; j < room.y + room.h ; j++) {
      if (type_stops_light(state->grid[i][j]) == 0) {
        // We check the visibility octant
        cell_coord.x = i;
        cell_coord.y = j;
        octant = wall_to_octant(state->player->pos, cell_coord);

        // We compute the corners
        corner_1.position.x = i * state->tile_screen_size - state->scroll.x;
        corner_1.position.y = j * state->tile_screen_size - state->scroll.y;

        corner_2.position.x = (i + 1) * state->tile_screen_size - state->scroll.x - 1;
        corner_2.position.y = j * state->tile_screen_size - state->scroll.y;

        corner_3.position.x = (i + 1) * state->tile_screen_size - state->scroll.x - 1;
        corner_3.position.y = (j + 1) * state->tile_screen_size - state->scroll.y - 1;

        corner_4.position.x = i * state->tile_screen_size - state->scroll.x;
        corner_4.position.y = (j + 1) * state->tile_screen_size - state->scroll.y - 1;

        // top edge of cell
        if (octant == NW || octant == N || octant == NE || octant == W || octant == E) {
          (*edges)[num_edge_tmp].a.position.x = corner_1.position.x;
          (*edges)[num_edge_tmp].a.position.y = corner_1.position.y;
          (*edges)[num_edge_tmp].b.position.x = corner_2.position.x;
          (*edges)[num_edge_tmp].b.position.y = corner_2.position.y;
        } else {
          (*edges)[num_edge_tmp].a.position.x = INFINITY;
        }
        num_edge_tmp++;
        // right edge of cell
        if (octant == N || octant == NE || octant == E || octant == SE || octant == S) {
          (*edges)[num_edge_tmp].a.position.x = corner_2.position.x;
          (*edges)[num_edge_tmp].a.position.y = corner_2.position.y;
          (*edges)[num_edge_tmp].b.position.x = corner_3.position.x;
          (*edges)[num_edge_tmp].b.position.y = corner_3.position.y;
        } else {
          (*edges)[num_edge_tmp].a.position.x = INFINITY;
        }
        num_edge_tmp++;
        // bot edge of cell
        if (octant == E || octant == SE || octant == S || octant == SW || octant == W) {
          (*edges)[num_edge_tmp].a.position.x = corner_3.position.x;
          (*edges)[num_edge_tmp].a.position.y = corner_3.position.y;
          (*edges)[num_edge_tmp].b.position.x = corner_4.position.x;
          (*edges)[num_edge_tmp].b.position.y = corner_4.position.y;
        } else {
          (*edges)[num_edge_tmp].a.position.x = INFINITY;
        }
        num_edge_tmp++;
        // left edge of cell
        if (octant == NW || octant == N || octant == S || octant == SW || octant == W) {
          (*edges)[num_edge_tmp].a.position.x = corner_4.position.x;
          (*edges)[num_edge_tmp].a.position.y = corner_4.position.y;
          (*edges)[num_edge_tmp].b.position.x = corner_1.position.x;
          (*edges)[num_edge_tmp].b.position.y = corner_1.position.y;
        } else {
          (*edges)[num_edge_tmp].a.position.x = INFINITY;
        }
        num_edge_tmp++;
      }
    }
  }

  // add 4 outer room walls
  // top edge of room
  (*edges)[num_edge_tmp].a.position.x = room.x * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].a.position.y = room.y * state->tile_screen_size - state->scroll.y;
  (*edges)[num_edge_tmp].b.position.x = (room.x + room.w) * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].b.position.y = room.y * state->tile_screen_size - state->scroll.y;
  num_edge_tmp++;
  // right edge of room
  (*edges)[num_edge_tmp].a.position.x = (room.x + room.w) * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].a.position.y = room.y * state->tile_screen_size - state->scroll.y;
  (*edges)[num_edge_tmp].b.position.x = (room.x + room.w) * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].b.position.y = (room.y + room.h) * state->tile_screen_size - state->scroll.y;
  num_edge_tmp++;
  // bot edge of room
  (*edges)[num_edge_tmp].a.position.x = (room.x + room.w) * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].a.position.y = (room.y + room.h) * state->tile_screen_size - state->scroll.y;
  (*edges)[num_edge_tmp].b.position.x = room.x * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].b.position.y = (room.y + room.h) * state->tile_screen_size - state->scroll.y;
  num_edge_tmp++;
  // left edge of room
  (*edges)[num_edge_tmp].a.position.x = room.x * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].a.position.y = (room.y + room.h) * state->tile_screen_size - state->scroll.y;
  (*edges)[num_edge_tmp].b.position.x = room.x * state->tile_screen_size - state->scroll.x;
  (*edges)[num_edge_tmp].b.position.y = room.y * state->tile_screen_size - state->scroll.y;
  // num_edge_tmp++;

  return num_edge;
}

// Comparison function for sorting vertices based on angles
int           compare_vertex_angles(const void* a, const void* b) {
  const       light_edge* va = (const light_edge*)a;
  const       light_edge* vb = (const light_edge*)b;

  if (va->angle_b == 1) {
    return -1;
  } else if (vb->angle_b == 1) {
    return 1;
  }
  if (va->angle_a < vb->angle_a) return -1;
  if (va->angle_a > vb->angle_a) return 1;
  return 0;
}

void          perform_shadow_casting(state_t *state) {
  light_edge      *blocking_light_edges = NULL;
  int             num_edge;
  SDL_Vertex      player_position;
  light_edge      *vertices = NULL;
  size_t          num_vertices = 0;
  size_t          capacity = 0;
  float           radius = 100.0;

  // initialize the array of blocking light edges
  num_edge = init_edges(state, &blocking_light_edges);

  player_position.position.x = state->player->dst_screen.x - state->scroll.x;
  player_position.position.y = state->player->dst_screen.y - state->scroll.y;
  add_vertex(&vertices, &num_vertices, &capacity, player_position.position.x, player_position.position.y, 200, 0);

  // Cast rays from the player's position to each edge
  for (int i = 0 ; i < num_edge ; i++) {
    light_edge current_edge = blocking_light_edges[i];
    if (current_edge.a.position.x == INFINITY)
      continue;
    
    // Iterate over both endpoints of the edge
    SDL_Vertex edge_endpoints[2] = {current_edge.a, current_edge.b};
    for (int k = 0; k < 2; k++) {
      SDL_Vertex endpoint = edge_endpoints[k];
        
      // Calculate the direction vector from player to endpoint
      float rdx, rdy;
      rdx = endpoint.position.x - player_position.position.x;
      rdy = endpoint.position.y - player_position.position.y;

      float base_ang = atan2f(rdy, rdx);
      float ang = 0;

      // Iterate over 3 rays: one directly at the point and two slightly offset
      for (int j = 0; j < 3; j++) {
        if (j == 0) ang = base_ang - 0.0001f;
        if (j == 1) ang = base_ang;
        if (j == 2) ang = base_ang + 0.0001f;

        // Create ray along angle for required distance
        rdx = radius * cosf(ang);
        rdy = radius * sinf(ang);

        float min_t1 = INFINITY;
        float min_px = 0, min_py = 0, min_ang = 0;
        int valid = 0;

        // Check for ray intersection with all edges
        for (int e = 0; e < num_edge; e++) {
          light_edge test_edge = blocking_light_edges[e];

          // Create line segment vector
          float sdx = test_edge.b.position.x - test_edge.a.position.x;
          float sdy = test_edge.b.position.y - test_edge.a.position.y;

          if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f) {
            // t2 is normalised distance from line segment start to line segment end of intersect point
            float t2 = (rdx * (test_edge.a.position.y - player_position.position.y) + (rdy * (player_position.position.x - test_edge.a.position.x))) / (sdx * rdy - sdy * rdx);
            // t1 is normalised distance from source along ray to ray length of intersect point
            float t1 = (test_edge.a.position.x + sdx * t2 - player_position.position.x) / rdx;

            // If intersect point exists along ray, and along line segment 
            // and closer than previous intersections, then it is valid
            if (t1 > 0 && t2 >= 0 && t2 <= 1.0f && t1 < min_t1) {
              min_t1 = t1;
              min_px = player_position.position.x + t1 * rdx;
              min_py = player_position.position.y + t1 * rdy;
              min_ang = atan2f(min_py - player_position.position.y, min_px - player_position.position.x);
              valid = 1;
            }
          }
        }
        // Add the closest intersection point to the visibility polygon
        if (valid == 1) {
          add_vertex(&vertices, &num_vertices, &capacity, min_px, min_py, 200, min_ang);
        }
      }
    }
  }
  // sort vertices by angle
  qsort(vertices, num_vertices, sizeof(light_edge), compare_vertex_angles);

  SDL_Vertex* final_vertices_array = malloc(num_vertices * sizeof(SDL_Vertex));
  if (final_vertices_array == NULL) {
    DEBUG_MSG("malloc failed for final_vertices_array");
    exit(EXIT_FAILURE);
  }

  // Extract SDL_Vertex instances from light_edge.a and store them in vertices_array
  for (size_t i = 0; i < num_vertices; i++) {
    final_vertices_array[i] = vertices[i].a;
  }

  SDL_SetTextureBlendMode(state->black_texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(state->renderer, state->black_texture);
  SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
  SDL_RenderClear(state->renderer);

  // prepare draw on texture
  size_t num_triangles = num_vertices - 2 + 1;
  size_t num_indices = num_triangles * 3; // Each triangle has 3 vertices
  int *indices = (int *)malloc(num_indices * sizeof(int)); // ensure alloc OK
  if (indices == NULL) {
    DEBUG_MSG("failed malloc for indices");
    exit(EXIT_FAILURE);
  }
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

  SDL_RenderGeometry(state->renderer, NULL, final_vertices_array, num_vertices, indices, num_indices);
  // add_torch_light(state);
  SDL_SetRenderTarget(state->renderer, NULL);
  free(blocking_light_edges);
  free(indices);
  free(vertices);
  free(final_vertices_array);
}
