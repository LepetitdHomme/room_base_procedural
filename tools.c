#include "includes/common.h"

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
