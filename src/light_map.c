#include <stdlib.h>
#include "../include/macros.h"
#include "../include/light_map.h"
#include "../include/type.h"

void          init_light_map(graph_t *node) {
  if (node->light_map != NULL) {
    free_light_map(node);
  }

  if ((node->light_map = (int **)malloc(sizeof(int *) * node->rect.w)) == NULL) {
    DEBUG_MSG("light map malloc failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0 ; i < node->rect.w; i++) {
    if ((node->light_map[i] = (int *)malloc(sizeof(int) * node->rect.h)) == NULL) {
      DEBUG_MSG("light map malloc i failed");
      exit(EXIT_FAILURE);
    }
  }
  // printf("light map: %d - %d\n", node->rect.w, node->rect.h);
  if (DEBUG_LIGHT_MAP) print_light_map(node);
}

void          print_light_map(graph_t *node) {
  if (node->light_map == NULL) {
    return;
  }

  for (int j = 0; j < node->rect.h; j++) {
    for (int i = 0 ; i < node->rect.w; i++) {
      printf("%d     ", node->light_map[i][j]);
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

/*            reset light map before ! */
void          update_light_map(state_t *state) {
  float       ray_x,ray_y,ray_angle,rad;
  int         angle,distance;
  coord_t     coord_ray;
  int         player_angle;

  rad = M_PI / 180;
  player_angle = direction_to_degrees(state->player->direction);

  // player vision angle -90 +90
  for (angle = player_angle - LIGHT_ANGLE / 2 ; angle < LIGHT_ANGLE / 2 ; angle++) {
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
      }
      // printf("coord_ray x: %d\n", coord_ray.x);
      // printf("x: %d,y: %d\n", coord_ray.x - state->player->current_node->rect.x, coord_ray.y - state->player->current_node->rect.y);
      state->player->current_node->light_map[coord_ray.x - state->player->current_node->rect.x][coord_ray.y - state->player->current_node->rect.y] = 100;
      if (type_stops_light(state->grid[coord_ray.x][coord_ray.y]) == 0) {
        break;
      }
    }
  }

  diffuse_light_map(state);
  // print_light_map(state->player->current_node);
}

void          diffuse_light_map(state_t *state) {
  graph_t     tmp_node;
  graph_t     *node = state->player->current_node;
  int         adjacent_tiles = 0;
  int         total_light;

  tmp_node.rect = node->rect;
  tmp_node.light_map = NULL;
  init_light_map(&tmp_node);
  // reset with current node light map values
  for (int i = 0 ; i < tmp_node.rect.w; i++) {
    for (int j = 0 ; j < tmp_node.rect.h; j++) {
      tmp_node.light_map[i][j] = node->light_map[i][j];
    }
  }
  // DEBUG_MSG("");

  // diffuse light in current node
  for (int i = 0 ; i < tmp_node.rect.w; i++) {
    for (int j = 0 ; j < tmp_node.rect.h; j++) {
      total_light = tmp_node.light_map[i][j];
      adjacent_tiles = 1;

      for (int k = -LIGHT_DETAIL ; k < LIGHT_DETAIL ; k++) {
        for (int l = -LIGHT_DETAIL ; l < LIGHT_DETAIL ; l++) {
          if (i+k >= 0 && i+k < node->rect.w && j+l >= 0 && j+l < node->rect.h) {
            total_light += node->light_map[i+k][j+l];
            adjacent_tiles += 1;
          }
        }
      }

      node->light_map[i][j] = total_light / adjacent_tiles;
    }
  }
  free_light_map(&tmp_node);
}
