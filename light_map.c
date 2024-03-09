#include "includes/common.h"

void          init_light_map(graph_t *node) {
  if (node->light_map != NULL) {
    free_light_map(node);
  }

  if ((node->light_map = (int **)malloc(sizeof(int *) * node->rect.w)) == NULL) {
    DEBUG_MSG("light map malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0 ; i < node->rect.w ; i++) {
    if ((node->light_map[i] = (int *)malloc(sizeof(int) * node->rect.h)) == NULL) {
      DEBUG_MSG("light map malloc i failed");
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0 ; i < node->rect.w ; i++) {
    for (int j = 0 ; j < node->rect.h; j++) {
      node->light_map[i][j] = 0; // meaning: cast dark opacity = 255 - light_map[i][j];
    }
  }
  print_light_map(node);
}

void          print_light_map(graph_t *node) {
  if (node->light_map == NULL) {
    return;
  }

  for (int i = 0 ; i < node->rect.w ; i++) {
    for (int j = 0; j < node->rect.h ; j++) {
      printf("%d", node->light_map[i][j]);
    }
    printf("\n");
  }
  printf("\n\n");
}

void          free_light_map(graph_t *node) {
  for (int i = 0 ; i < node->rect.w; i++) {
    free(node->light_map[i]);
    node->light_map[i] = NULL;
  }
  free(node->light_map);
  node->light_map = NULL;
}


void          reset_light_map(graph_t *node) {
  if (node->light_map == NULL) {
    return;
  }

  for (int i = 0 ; i < node->rect.w; i++) {
    for (int j = 0 ; j < node->rect.h; j++) {
      node->light_map[i][j] = 0;
    }
  }
}

void          update_light_map(state_t *state) {
  float       ray_x,ray_y,ray_angle,rad;
  int         angle,distance;
  coord_t     coord_ray;

  rad = M_PI / 180;

  for (angle = 0 ; angle < 360 ; angle++) {
    ray_angle = angle * rad;
    // cast ray
    for (distance = 0; distance < state->player->light ; distance++) {
      ray_x = state->player->dst_screen.x + distance * cos(ray_angle);
      ray_y = state->player->dst_screen.y + distance * sin(ray_angle);

      // change to use props instead
      coord_ray.x = ray_x / state->tile_screen_size;
      coord_ray.y = ray_y / state->tile_screen_size;
      // printf("player: %d, %d\n", state->player->dst_screen.x, state->player->dst_screen.y);
      // print_rect(state->player->current_node->rect, "node");
      if (is_in_room(coord_ray, state->player->current_node->rect) != 0) {
        break;
      } else {
        // printf("x: %d,y: %d\n", coord_ray.x - state->player->current_node->rect.x, coord_ray.y - state->player->current_node->rect.y);
        state->player->current_node->light_map[coord_ray.x - state->player->current_node->rect.x][coord_ray.y - state->player->current_node->rect.y] = 255;
        if (type_stops_light(state->grid[coord_ray.x][coord_ray.y]) == 0) {
          break;
        }
      }
    }
  }
}

