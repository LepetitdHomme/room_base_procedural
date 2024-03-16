#include "../include/tools.h"
#include "../include/macros.h"

int           clamp(int value, int min, int max) {
  return (value < min) ? min : (value > max) ? max : value;
}

int           random_int(int lower, int upper) {
  int random = (rand() % (upper - lower + 1)) + lower;
  return random;
}

double        distance_between_coords(coord_t center_1, coord_t center_2) {
  return sqrt(pow(center_2.x - center_1.x, 2) + pow(center_2.y - center_1.y, 2));
}

// more efficient
double        squared_distance_between_coords(coord_t center_1, coord_t center_2) {
  double  distance = pow(center_2.x - center_1.x, 2) + pow(center_2.y - center_1.y, 2);
  return distance;
}

void          print_rect(SDL_Rect rect, char *name) {
  printf("print_rect: %s: %d - %d | %d - %d\n", name, rect.x, rect.y, rect.w, rect.h);
}


Uint8         alpha_light(int light, double distance) {
  Uint8       alpha;
  double      light_squared;

  alpha = 0;
  light_squared = light * light;

  if (distance <= light_squared) {
    alpha = (Uint8)(((double)(light_squared - distance) / light_squared) * 255);
  }
  // printf("distane: %f, alpha: %u\n", distance, alpha);
  return alpha;
}

coord_t find_intersection(coord_t p1, coord_t p2, line_t lines[], int size_lines) {
  coord_t intersection = {-1, -1};

  // Calculate the slope of the segment p1, p2
  float segment_slope;
  if (p1.x == p2.x) {
    segment_slope = 1.0 / 0.0; // Vertical line
  } else {
    segment_slope = (float)(p2.y - p1.y) / (p2.x - p1.x);
    // printf("slope: %f\n", segment_slope);
  }

  for (int i = 0; i < size_lines; i++) {
    if (lines[i].a.x == -1) {
      continue; // Skip invalid lines
    }

    // Check if the line is vertical
    if (lines[i].a.x == lines[i].b.x) {
      // Check if the segment is also vertical
      if (p1.x == p2.x) {
        if (p1.x == lines[i].a.x) {
          // Lines overlap, return one of the common points
          intersection.x = p1.x;
          intersection.y = (p1.y < p2.y) ? p2.y : p1.y;
          return intersection;
        }
      }
    }

    // Check if the line is horizontal
    if (lines[i].a.y == lines[i].b.y) {
      // Check if the segment is also horizontal
      if (p1.y == p2.y) {
        if (p1.y == lines[i].a.y) {
          // Lines overlap, return one of the common points
          intersection.x = (p1.x < p2.x) ? p2.x : p1.x;
          intersection.y = p1.y;
          return intersection;
      }
      }
    }

    // Check if the segment intersects with a vertical line
    if (lines[i].a.x == lines[i].b.x) {
      float line_x = lines[i].a.x;
      float intersect_y = p1.y + (line_x - p1.x) * segment_slope;
      if (lines[i].a.y > lines[i].b.y) {
        if (intersect_y <= lines[i].a.y && intersect_y >= lines[i].b.y) {
          intersection.x = line_x;
          intersection.y = intersect_y;
          return intersection;
        }
      } else {
        if (intersect_y >= lines[i].a.y && intersect_y <= lines[i].b.y) {
          intersection.x = line_x;
          intersection.y = intersect_y;
          return intersection;
        }
      }
    }

    // Check if the segment intersects with a horizontal line
    if (lines[i].a.y == lines[i].b.y) {
      float line_y = lines[i].a.y;
      float intersect_x = p1.x + (line_y - p1.y) / segment_slope;
      if (lines[i].a.x > lines[i].b.x) {
        if (intersect_x <= lines[i].a.x && intersect_x >= lines[i].b.x) {
          intersection.x = intersect_x;
          intersection.y = line_y;
          return intersection;
        }
      } else {
        if (intersect_x >= lines[i].a.x && intersect_x <= lines[i].b.x) {
          intersection.x = intersect_x;
          intersection.y = line_y;
          return intersection;
        }
      }
    }
  }

  return intersection;
}

coord_t       extend_segment(coord_t player, coord_t corner) {
  coord_t     extended_coord;

  // Calculate the direction of the segment
  float dx = corner.x - player.x;
  float dy = corner.y - player.y;

  // Extend the segment indefinitely in the direction of the corner
  extended_coord.x = corner.x + dx * 10; // Extend far enough
  extended_coord.y = corner.y + dy * 10;

  return extended_coord;
}

coord_t       find_intersection_with_walls(state_t *state, coord_t player, coord_t corner, enum Octant octant) {
  SDL_Rect    rect = state->player->current_node->rect;
  coord_t     corner1,corner2,corner3,corner4,invalid;
  line_t      wall_1,wall_2,wall_3;
  line_t      lines[3];
  // printf("octant: %d\n", octant);
  // printf("corner: %d,%d\n", corner.x, corner.y);

  // top left
  corner1.x = rect.x * state->tile_screen_size - state->scroll.x;
  corner1.y = rect.y * state->tile_screen_size - state->scroll.y;
  // top right
  corner2.x = (rect.x + rect.w) * state->tile_screen_size - state->scroll.x;
  corner2.y = rect.y * state->tile_screen_size - state->scroll.y;
  // bot right
  corner3.x = (rect.x + rect.w) * state->tile_screen_size - state->scroll.x;
  corner3.y = (rect.y + rect.h) * state->tile_screen_size - state->scroll.y;
  // bot left
  corner4.x = rect.x * state->tile_screen_size - state->scroll.x;
  corner4.y = (rect.y + rect.h) * state->tile_screen_size - state->scroll.y;

  invalid.x = -1;
  invalid.y = -1;

  if (octant == E) {
    wall_1.a = corner1;
    wall_1.b = corner2;
    wall_2.a = corner2;
    wall_2.b = corner3;
    wall_3.a = corner3;
    wall_3.b = corner4;
  } else if (octant == NE) {
    wall_1.a = corner1;
    wall_1.b = corner2;
    wall_2.a = corner2;
    wall_2.b = corner3;
    wall_3.a = invalid;
    wall_3.b = invalid;      
  } else if (octant == N) {
    wall_1.a = corner4;
    wall_1.b = corner1;
    wall_2.a = corner1;
    wall_2.b = corner2;
    wall_3.a = corner2;
    wall_3.b = corner3;
  } else if (octant == NW) {
    wall_1.a = corner4;
    wall_1.b = corner1;
    wall_2.a = corner1;
    wall_2.b = corner2;
    wall_3.a = invalid;
    wall_3.b = invalid;
  } else if (octant == W) {
    wall_1.b = corner3;
    wall_1.a = corner4;
    wall_2.a = corner4;
    wall_2.b = corner1;
    wall_3.a = corner1;
    wall_3.b = corner2;
  } else if (octant == SW) {
    wall_1.a = corner3;
    wall_1.b = corner4;
    wall_2.a = corner4;
    wall_2.b = corner1;
    wall_3.a = invalid;
    wall_3.b = invalid;
  } else if (octant == S) {
    wall_1.a = corner2;
    wall_1.b = corner3;
    wall_2.a = corner3;
    wall_2.b = corner4;
    wall_3.a = corner4;
    wall_3.b = corner1;
  } else {
    wall_1.a = corner2;
    wall_1.b = corner3;
    wall_2.a = corner3;
    wall_2.b = corner4;
    wall_3.a = invalid;
    wall_3.b = invalid;
  }

  lines[0] = wall_1;
  lines[1] = wall_2;
  lines[2] = wall_3;
  return find_intersection(player, corner, lines, 3);
}

